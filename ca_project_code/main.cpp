/*
    Uncoomment the include libs in this comment block to run in the arduino IDE.
    The resources folder has all the resources/libraries below to inlcude into the
    arduino library manager to run the code as a cnc drawing code. 

    // The A4988.h lib is used to control the steppers, it is a board that gives fine
    // control over the steppers movement.
    #include <A4988.h>

    // The BasicStepperDriver.h lib is used to control the stepper motors
    #include <BasicStepperDriver.h>\

    // Servo.h is used to control the servo of the pen
    #include <Servo.h>

    // SyncDriver.h gives a finer control of the motors, with making them move in a 
    // syncronized movement for smoother drawing.
    #include <SyncDriver.h>

    // ezButton.h is the lib for dealing with the limit switches
    #include <ezButton.h>

    // Stepper.h is the the Arduino lib for stepper motor control. unused
    #include <Stepper.h>

    // gcode.h is the lib for accessing the information from the serial bus 
    // creating a buffer of commands to be read and stepped through to draw
    // gives you the X, Y, and Z cords to go to, in our case move the Pen up and down.
    #include <gcode.h>

*/
#include "libs/A4988.h"
#include "libs/BasicStepperDriver.h"
#include "libs/MultiDriver.h"
#include "libs/SyncDriver.h"
#include "libs/ezButton.h"
#include "libs/Stepper.h"
#include "libs/gcode.h"
#include "libs/Servo.h"



// #define Steps 32

#define Num_Commands 2

// The number of steps per the revolution of the motor 
#define Motor_Steps 200
// Number of steps to go 1 millimeter
#define Steps_mm 80
// target RPM set for x-axis and y-axis motor
#define Motor_xaxis_RPM 60
#define Motor_yaxis_RPM 60

// set up for the x-axis motor
#define Dir_x 5
#define Step_x 2
// set up for the y-axis motor
#define Dir_y 6
#define Step_y 3

// check to enable functionality between devices by checking the pin is connected
#define Enable 8

// credit for code 
// If microstepping is set externally, make sure this matches the selected mode
// 1=full step, 2=half step, 4 = 1/4, 8 = 1/8 and 16 = 1/6 Microsteps.
#define MicroSteps 16


// Hard limits are set to a 8x11 inch sheet of paper. 
// setting hardlimit for the x axis in mm
#define HARD_X_LIMIT 203.2
// setting harlimit for the y axis in mm
#define HARD_Y_LIMIT 279.4


// initialize the x and y-axis motors
A4988 x_axis_motor(Motor_Steps, Dir_x, Step_x, Enable);
A4988 y_axis_motor(Motor_Steps, Dir_y, Step_y, Enable);

// creating a sync between both motors to move intandem to each other
SyncDriver controller(x_axis_motor, y_axis_motor);

// creating my limit switch buttons to limit the movement of the motors 
// creating a safety buffer. 
ezButton limitSwitch_1(9); // pin will be the number of pin used for switch
ezButton limitSwitch_2(10); // ...

// creating servo for pen 
Servo zPen;

// this brings the machine back to the home position
void homing();
commandscallback comnd[1] = {{"g28", homing}};
gcode comnds(1, comnd);

void gotoLocation(double x, double y);
void calibrate();

// setting up count for hard limits for the x and y axis. 
double xCount;
double yCount;

double X;
double Y;
double Z;

void setup() {
    // setting pin for servo
    zPen.attach(11);
    //Enabling pin mode for the enable pin
    pinMode(Enable, OUTPUT);
    // SETTING UP MOTORS X AND Y 
    x_axis_motor.begin(Motor_xaxis_RPM, MicroSteps);
    y_axis_motor.begin(Motor_yaxis_RPM, MicroSteps);
    // SETTING X AND Y ENABLE ACTIVE STATES 
    x_axis_motor.setEnableActiveState(LOW);
    y_axis_motor.setEnableActiveState(LOW);
    // SETTING THE LIMIT SWITCH DEBOUNCE TIME
    limitSwitch_1.setDebounceTime(50);
    limitSwitch_2.setDebounceTime(50);
    //calling homing, and will be called a second, causing a bounce, but making sure it is 
    // accurately zeroed.
    homing();
    comnds.begin();
    
    
}

void loop() 
{
    // This checks if there are any commands to run
    if(comnds.available())
    {
        double nextX;
        double nextY;
        double changeZ;

        // checks for the M gcode to determine if the pen is up or down
        if(comnds.availableValue('Z'))
        {
            // recieve the pen value to move servo
            changeZ = comnds.GetValue('Z');
            // checking the value up if greater than 2 and down if less
            if(changeZ > 2)
            {
                // checking the value the servo is at before we change 
                // if already set leave alone
                if(zPen.read() != 0)
                {
                    zPen.write(0);
                    delay(30);
                }
            }
            else
            {
                // checking the value the servo is at before we change 
                // if already set leave alone
                if(zPen.read() != 90)
                {
                    zPen.write(90);
                    delay(30);
                }
            }
            
        }
        else
        {
            // Checks for a X command in buffer, then gets the value if true
            if(comnds.availableValue('X'))
                nextX = comnds.GetValue('X');
            // Checks for a Y command in buffer, then gets the value if true
            if(comnds.availableValue('Y'))
                nextY = comnds.GetValue('Y');
            // once its found the values it sends it to the gotolocation
            gotoLocation(nextX, nextY);
        }
        
    }
}

/*
    Functoin: homing()
    Parameter(s): none
    Method: homing() is used to find the home position of the cnc before it starts to run
    the gcode. Allowing for it to be at the orgin of the xy-plane (0,0). The homing function first 
    checks if the pen is up or down, moving it to the up position if it is not already.     
*/
void homing(){
    int stepfor = 1 * Steps_mm;
    int stepbac = -1 * Steps_mm;

    if(zPen.read() > 1)
        zPen.write(0);

    while(limitSwitch_1.isReleased() || limitSwitch_2.isReleased())
    {
        if(limitSwitch_1.isReleased())
            if(limitSwitch_2.isReleased())
                controller.move(stepbac,stepbac);
            else
                controller.move(stepbac, 0);
        else
        {
            if(limitSwitch_2.isReleased())
                controller.move(0, stepbac);
        }
    }
    while(limitSwitch_1.isPressed() && limitSwitch_2.isPressed())
    {
        controller.move(stepfor, stepfor);
    }
    if(limitSwitch_1.isPressed()||limitSwitch_2.isPressed())
    {
        exit(Serial.println("Error, limit homing failed"));
    }
    
    X = 0;
    Y = 0;
}
/*
    Function: gotoLocation()
    Parameter(s): double x, double y
    Method: This function takes to x, y values then calculates the distance between them 
    and then finds the number steps that each moter in that axis needs to step to get to the 
    location. It steps the motor in milimeters(mm).
    return: none/void
*/
void gotoLocation(double x, double y)
{
    xCount += (x - X);
    yCount += (y - Y);
    int numStepsX;
    int numStepsY;
    
    if(xCount < HARD_X_LIMIT)
        numStepsX = (x - X)*Steps_mm;
    else
        numStepsX = 0;
    if(yCount < HARD_Y_LIMIT) 
        numStepsY = (y - Y)*Steps_mm;
    else
        numStepsY = 0;

    x_axis_motor.enable();
    y_axis_motor.enable();

    controller.move(numStepsX, numStepsY);

    X = x;
    Y = y;

    x_axis_motor.disable();
    y_axis_motor.disable();
    
    // comnds.comment("") send back commands or where it is 
    // for voice 

}
