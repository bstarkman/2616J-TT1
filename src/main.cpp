#include "main.h"
#include "2616J.h"
#include "string.h"


/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

 MotorGroup leftmg({ MOTOR_DRIVE_FRONT_LEFT,MOTOR_DRIVE_BACK_LEFT});
 MotorGroup rightmg( {-MOTOR_DRIVE_FRONT_RIGHT,-MOTOR_DRIVE_BACK_RIGHT});

 MotorGroup drive1 ({-MOTOR_DRIVE_FRONT_LEFT, -MOTOR_DRIVE_BACK_LEFT, -MOTOR_DRIVE_FRONT_RIGHT, -MOTOR_DRIVE_BACK_RIGHT});
 MotorGroup turnLeft ({-MOTOR_DRIVE_FRONT_LEFT, -MOTOR_DRIVE_BACK_LEFT, MOTOR_DRIVE_FRONT_RIGHT, MOTOR_DRIVE_BACK_RIGHT});
 MotorGroup turnRight({MOTOR_DRIVE_FRONT_LEFT, MOTOR_DRIVE_BACK_LEFT, -MOTOR_DRIVE_FRONT_RIGHT, -MOTOR_DRIVE_BACK_RIGHT});

 MotorGroup intake({MOTOR_INTAKE_RIGHT,-MOTOR_INTAKE_LEFT});

 MotorGroup lift({-MOTOR_LIFT});

 Motor angler(-MOTOR_ANGLER);

 //------------------------------------------------------------------------------

 auto drive = ChassisControllerFactory::create(
 	leftmg, rightmg,
   AbstractMotor::gearset::green,
   {4_in, 9.0_in}
 //distance between the center of the front drive wheels
 );

void autonomous() {

angler.moveVoltage(-1000);
lift.moveVoltage(-1000);

drive.setMaxVelocity(100);
drive.moveDistance(100);

pros::delay(100);

drive.setMaxVelocity(100);
drive.moveDistance(-100);

pros::delay(100);

intake.moveVoltage(12000);
pros::delay(100);

drive.setMaxVelocity(100);
drive.moveDistance(1100);

pros::delay(100);

drive.setMaxVelocity(100);
drive.moveDistance(-375);


drive.setMaxVelocity(75);
drive.turnAngle(-370);

drive.setMaxVelocity(100);
drive.moveDistance(750);


if (abs(angler.getPosition())<=1300) {
	 angler.moveVoltage(12000);

} else if (abs(angler.getPosition())>1300 && abs(angler.getPosition())<=1500) {
 angler.moveVoltage(6000);

} else if(abs(angler.getPosition())>1500) {
 angler.moveVoltage(3000);
}
pros::delay(200);

drive.moveDistance(-500);

}

/*
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operatorS
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */

 // Chassis Control9ler - lets us drive the robot around with open- or closed-loop control

void opcontrol() {

//------------------------------------------------------------------------------
char s[30];
	okapi::Controller masterController;

 ControllerButton IntakeInButton(ControllerDigital::R1);
 ControllerButton IntakeOutButton(ControllerDigital::R2);

 ControllerButton AnglerUpButton(ControllerDigital::up);
 ControllerButton AnglerDownButton(ControllerDigital::down);

 ControllerButton LiftUpButton(ControllerDigital::L1);
 ControllerButton LiftDownButton(ControllerDigital::L2);
//------------------------------------------------------------------------------
intake.setBrakeMode(AbstractMotor::brakeMode::brake);
angler.setBrakeMode(AbstractMotor::brakeMode::brake);
lift.setBrakeMode(AbstractMotor::brakeMode::brake);
angler.tarePosition();
int count =0;
count++;
//------------------------------------------------------------------------------
	while (true) {
		drive.tank(masterController.getAnalog(ControllerAnalog::leftY),
						masterController.getAnalog(ControllerAnalog::rightY));
//------------------------------------------------------------------------------
   if(IntakeInButton.isPressed())
	 {
		 intake.moveVoltage(12000);
		 lift.moveVoltage(-1000);

		 //resets counter to 0
	 } else if(IntakeOutButton.isPressed() && count < 1){
		  intake.moveVoltage(-6000);
	 count =0;
	 }else if(IntakeOutButton.isPressed() && count >= 1){
		 intake.moveVoltage(-12000);

	 }else{
		 intake.moveVoltage(0);

	 }
//------------------------------------------------------------------------------
  if(LiftUpButton.isPressed() && abs(angler.getPosition())<600){
      angler.moveVoltage(12000);
			pros::delay(100);
			lift.moveVoltage(12000);
			//cout  << "button;"

	}else if(LiftUpButton.isPressed()){
		lift.moveVoltage(12000);

	}else if(LiftDownButton.isPressed()){
    lift.moveVoltage(-12000);

	}else{
		lift.moveVoltage(-800);
	}
//------------------------------------------------------------------------------
 if(AnglerUpButton.isPressed())
	 {
	   //sprintf(s,"%f",angler.getPosition());
  	 //masterController.setText(0, 0, s);
     //Display position of (MotorGroup) on controller
		 if (abs(angler.getPosition())<=1400) {
			  angler.moveVoltage(12000);

		 } else if (abs(angler.getPosition())>1400 && abs(angler.getPosition())<=1550) {
 		 	angler.moveVoltage(6000);

		} else if(abs(angler.getPosition())>1550) {
			angler.moveVoltage(3000);
		}

	 } else if(AnglerDownButton.isPressed()){
		  angler.moveVoltage(-12000);

	 } else if(LiftDownButton.isPressed()){
		 pros::delay(375);
		 angler.moveVoltage(-12000);

	 }else{
		 angler.moveVoltage(0);
	 }
//------------------------------------------------------------------------------
	pros::delay(20);
 }
}

/**

Broken Ports:

TO DO LIST
1. Tray control with angler movement
2. IntakeOut Speed ajustments

cd C:\Users\bstar\git\2616J\2616J-TT1
prosv5 make
prosv5 upload --slot 2 --name TTNew
*/
