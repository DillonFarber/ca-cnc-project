/*
    Uncoomment the include libs in this comment block to run in the arduino IDE.
    The resources folder has all the resources/libraries below to inlcude into the
    arduino library manager to run the code as a cnc drawing code. 


    #include <A4988.h>
    #include <BasicStepperDriver.h>
    #include <Servo.h>
    #include <MultiDriver.h>
    #include <SyncDriver.h>
    #include <ezButton.h>
    #include <Stepper.h>
    #include <gcode.h>
    #include "config.h"
    #include <Servo.h>
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
    //homing();
    comnds.begin();
    
    
}

void loop() 
{
    if(comnds.available())
    {
        double nextX;
        double nextY;
        double nextZ; 
    
        if(comnds.availableValue('X'))
            nextX = comnds.GetValue('X');
        if(comnds.availableValue('Y'))
            nextY = comnds.GetValue('Y');
        if(comnds.availableValue('Z'))
            nextZ = comnds.GetValue('Z');

        // moving ben up and down depending on 
        // Z depth. 
        if(nextZ > 0)
        {
            zPen.write(45);
            delay(15);
        }
        else
        {
            zPen.write(0);
            delay(15);
        }

        gotoLocation(nextX, nextY);

        Z = nextZ;
    }
}

void homing(){
    while(limitSwitch_1.isReleased())
    {
        x_axis_motor.move((-1 * Steps_mm));
        if(limitSwitch_1.isPressed())
            {
                x_axis_motor.move(1);
                break;
            }
    }
    while(limitSwitch_2.isReleased())
    {
        y_axis_motor.move((-1 * Steps_mm));
        if(limitSwitch_2.isPressed())
            {
                y_axis_motor.move(1);
                break;
            }
    }
    X = 0;
    Y = 0;
    Z = 0;
}

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
