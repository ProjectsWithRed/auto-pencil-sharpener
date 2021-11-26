#include <Stepper.h>

#define motorInterfaceType 1


const int STEPPER_STEP_PIN = 5;
const int STEPPER_DIR_PIN = 4;

const int STEPPER_POT_PIN = A3;  // To control the speed of the stepper motor.

// To control the stepper motor left and right movement.
const int STEPPER_L_BTN_PIN = 2;
const int STEPPER_R_BTN_PIN = 1;

const int STEPPER_STEPS = 1;  // Stepper steps movement at a time.

const int DCMOTOR_MOSFET_PIN = 7;



Stepper stepper(200, STEPPER_DIR_PIN, STEPPER_STEP_PIN);



int stepperSpeed = 200;
int prevStepperSpeed = stepperSpeed;  // Previous stepper speed.

// Current position/step of the stepper motor, where 0 is all the way to the right.
int stepperCurrStep = 0;

// Start and end positions of the stepper motor, this is to ensure that the stepper motor stops before linear rail ends.
int stepperStartSteps = 0;
int stepperEndSteps = -26000;



void setup() {
    stepper.setSpeed(stepperSpeed);

    pinMode(STEPPER_L_BTN_PIN, INPUT_PULLUP);
    pinMode(STEPPER_R_BTN_PIN, INPUT_PULLUP);

    pinMode(DCMOTOR_MOSFET_PIN, OUTPUT);
}


void handleStepper() {
    getStepperSpeed();
    updateStepperSpeed(stepperSpeed);

    int stepperLBtn = digitalRead(STEPPER_L_BTN_PIN);
    int stepperRBtn = digitalRead(STEPPER_R_BTN_PIN);

    if(stepperLBtn == 0 && stepperCurrStep >= stepperEndSteps) {
        stepper.step(-STEPPER_STEPS);
        stepperCurrStep -= STEPPER_STEPS;
        handleDCMotor(1);
    } else if(stepperRBtn == 0 && stepperCurrStep < stepperStartSteps) {        
        stepper.step(STEPPER_STEPS);
        stepperCurrStep += STEPPER_STEPS;
        handleDCMotor(0);
    } else {
        stepper.step(0);
        handleDCMotor(0);
    } 
}


void getStepperSpeed() {
    int stepperPotVal = analogRead(STEPPER_POT_PIN) / 100;
    stepperSpeed = map(stepperPotVal, 0, 10, 50, 2000);
    stepperSpeed = stepperSpeed;
}


void updateStepperSpeed(int newStepperSpeed) {    
    if(newStepperSpeed != prevStepperSpeed) {
        stepper.setSpeed(newStepperSpeed);
        prevStepperSpeed = newStepperSpeed;
    }
}


void handleDCMotor(bool dcMotorSpin) {
    digitalWrite(DCMOTOR_MOSFET_PIN, dcMotorSpin);
}


void loop() {
    handleStepper();
}
