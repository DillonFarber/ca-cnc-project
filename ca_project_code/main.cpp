// #include <A4988.h>
// #include <BasicStepperDriver.h>
// #include <DRV8825.h>
// #include <DRV8834.h>
// #include <DRV8880.h>
// #include <MultiDriver.h>
// #include <SyncDriver.h>
// #include <ezButton.h>

// #include <Stepper.h>
// #include <gcode.h>
// #include "config.h"
// uncomment the previous include libs for Arduino IDE
// Delete the following from the file for the Arduino IDE
#include "libs/A4988.h"
#include "libs/BasicStepperDriver.h"
#include "libs/DRV8825.h"
#include "libs/DRV8834.h"
#include "libs/DRV8880.h"
#include "libs/MultiDriver.h"
#include "libs/SyncDriver.h"
#include "libs/ezButton.h"
#include "libs/Stepper.h"
#include


#define Steps 32

#define Num_Commands 2

// The number of steps per the revolution of the motor 
#define Motor_Steps 200
// Number of steps to go 1 millimeter
#define Steps_mm 80
// target RPM set for x-axis and y-axis motor
#define Motor_xaxis_RPM 120
#define Motor_yaxis_RPM 120

// set up for the x-axis motor
#define Dir_x 5
#define Step_x 2
// set up for the y-axis motor
#define Dir_y 6
#define Step_y 2

// check to enable functionality between devices by checking the pin is connected
#define Enable 8

// credit for code 
// If microstepping is set externally, make sure this matches the selected mode
// 1=full step, 2=half step, 4 = 1/4, 8 = 1/8 and 16 = 1/6 Microsteps.
#define MicroSteps 16

// initialize the x and y-axis motors
A4988 x_axis_motor(Motor_Steps, Dir_x, Step_x, Enable);
A4988 y_axis_motor(Motor_Steps, Dir_y, Step_y, Enable);

// creating a sync between both motors to move intandem to each other
SyncDriver controller(x_axis_motor, y_axis_motor);

// creating my limit switch buttons to limit the movement of the motors 
// creating a safety buffer. 
ezButton limitSwitch_1(pin); // pin will be the number of pin used for switch
ezButton limitSwitch_2(pin); // ...


// this brings the machine back to the home position
void homing();

void movement();
void gotoLocation(double x, double y);
void calibrate();

double X = 0.0;
double Y = 0.0;

void setup() {
    Serial.begin(115200);
    Serial.println("Ready!");
    limitSwitch_1.setDebounceTime(50);
    limitSwitch_2.setDebounceTime(50);
}

void loop() {
    limitSwitch_1.loop();

}

void homing{
  
}
void movement(){
}
void gotoLocation(double x, double y){
}
void calibrate(){

}