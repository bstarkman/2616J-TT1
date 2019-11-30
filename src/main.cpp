#include "main.h"
#include "2616J.h"
#include "string.h"
#include "pros/apix.h"

#include "display/lvgl.h"
#include "display/lv_conf.h"



//lv_img_dsc_t red_flower;


/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
 MotorGroup leftmg({ MOTOR_DRIVE_FRONT_LEFT,MOTOR_DRIVE_BACK_LEFT});
 MotorGroup rightmg( {-MOTOR_DRIVE_FRONT_RIGHT,-MOTOR_DRIVE_BACK_RIGHT});

 MotorGroup drive1 ({-MOTOR_DRIVE_FRONT_LEFT, -MOTOR_DRIVE_BACK_LEFT, -MOTOR_DRIVE_FRONT_RIGHT, -MOTOR_DRIVE_BACK_RIGHT});
 MotorGroup turnLeft ({-MOTOR_DRIVE_FRONT_LEFT, -MOTOR_DRIVE_BACK_LEFT, MOTOR_DRIVE_FRONT_RIGHT, MOTOR_DRIVE_BACK_RIGHT});
 MotorGroup turnRight({MOTOR_DRIVE_FRONT_LEFT, MOTOR_DRIVE_BACK_LEFT, -MOTOR_DRIVE_FRONT_RIGHT, -MOTOR_DRIVE_BACK_RIGHT});

  Motor angler(-MOTOR_ANGLER);

	MotorGroup intake({MOTOR_INTAKE_RIGHT,-MOTOR_INTAKE_LEFT});

  MotorGroup lift({-MOTOR_LIFT});

	auto drive = ChassisControllerFactory::create(
	 leftmg, rightmg,
		AbstractMotor::gearset::green,
		{4_in, 9.0_in}
	//distance between the center of the front drive wheels9
	);
int x;
okapi::Controller masterController;
 void my_task_fn(void* param) {
	 char s[30];
	 while(1==1){

	 sprintf(s,"AE:%05.1f IE:%05.1f CE%d ",angler.getEfficiency(), intake.getEfficiency(),x++);
	// masterController.setText(2,1, "               ");
	 masterController.setText(2, 1, s);
	 pros::lcd::set_text(6, s);
	 pros::delay(100);
 }
	 //Display position of (MotorGroup) on controller
 }

void initialize() {
  // lv_init();

  pros::delay(100);
  	/*************************
  	 * IMAGE FROM SOURCE CODE
  	 *************************/
    LV_IMG_DECLARE(red_flower);
  	lv_obj_t * img_var = lv_img_create(lv_scr_act(), NULL); /*Crate an image object*/
  	lv_img_set_src(img_var, &red_flower);  /*Set the created file as image (a red flower)*/
  	lv_obj_set_pos(img_var, 0, 0); // actually 76 but w/e
  	lv_obj_align(img_var, lv_scr_act(), LV_ALIGN_CENTER, 0,0);
    lv_obj_t * progname = lv_label_create(lv_scr_act(), NULL);
  	lv_label_set_align(progname, LV_LABEL_ALIGN_CENTER);
  	lv_obj_set_width(progname, 150);
  	lv_label_set_text(progname, "RedStack");
  	lv_obj_set_pos(progname,95,50);
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

 //------------------------------------------------------------------------------

 // auto drive = ChassisControllerFactory::create(
 // 	leftmg, rightmg,
 //   AbstractMotor::gearset::green,
 //   {4_in, 9.0_in}
 // //distance between the center of the front drive wheels
 // );

void autonomous() {

angler.moveVoltage(-1000);
lift.moveVoltage(-1200);

drive.setMaxVelocity(100);
drive.moveDistance(100); //forward

pros::delay(100);

drive.setMaxVelocity(100);
drive.moveDistance(-100); //backward

intake.moveVoltage(-12000); //outake

pros::delay(1000);

intake.moveVoltage(12000); //intake
pros::delay(150);

drive.setMaxVelocity(100);
drive.moveDistance(1450);//forward

pros::delay(100);

drive.setMaxVelocity(100);
drive.moveDistance(-500);//backward

intake.moveVoltage(0);//stop intake

drive.setMaxVelocity(75);
drive.turnAngle(355);//turn left

drive.setMaxVelocity(100);
drive.moveDistance(730);//forward

intake.moveVoltage(-4000);
pros::delay(480);
intake.moveVoltage(0);

if (abs(angler.getPosition())<=1300) {//stack the cubes
	 angler.moveVoltage(12000);

} else if (abs(angler.getPosition())>1300 && abs(angler.getPosition())<=1500) {
 angler.moveVoltage(3000);

} else if(abs(angler.getPosition())>1500) {
 angler.moveVoltage(1000);
}
pros::delay(2500);

drive.moveDistance(-500);//move backward

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

 pros::Task my_cpp_task (my_task_fn, (void*)"PROS", "My Task");


 ControllerButton IntakeInButton(ControllerDigital::R1);
 ControllerButton IntakeOutButton(ControllerDigital::R2);

 ControllerButton AnglerUpButton(ControllerDigital::up);
 ControllerButton AnglerDownButton(ControllerDigital::down);

 ControllerButton LiftUpButton(ControllerDigital::L1);
 ControllerButton LiftDownButton(ControllerDigital::L2);

  ControllerButton DoubleTower(ControllerDigital::A);
//------------------------------------------------------------------------------
intake.setBrakeMode(AbstractMotor::brakeMode::brake);
angler.setBrakeMode(AbstractMotor::brakeMode::brake);
lift.setBrakeMode(AbstractMotor::brakeMode::brake);
//angler.tarePosition();
int count =0;
//------------------------------------------------------------------------------
	while (true) {
		drive.tank(masterController.getAnalog(ControllerAnalog::leftY),
						masterController.getAnalog(ControllerAnalog::rightY));
//------------------------------------------------------------------------------
   if(IntakeInButton.isPressed())
	 {
		 intake.moveVoltage(12000);
		 lift.moveVoltage(-1000);
     count =0;
		 //resets counter to 0

	} else if(IntakeOutButton.isPressed()){
			count++;

	 if(count < 100){
		 intake.moveVoltage(-6000);

	 }else if(IntakeOutButton.isPressed()){
		 intake.moveVoltage(-12000);

	 }else if(IntakeInButton.isPressed()){
		 count=0;
	 }

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
		lift.moveVoltage(-800);//runs down lift constantly
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

		} else if(abs(angler.getPosition())>1450) {
			angler.moveVoltage(3000);
		}

	 } else if(AnglerDownButton.isPressed()){
		  angler.moveVoltage(-12000);

	 } else if(LiftDownButton.isPressed()){
		 pros::delay(300);
		 angler.moveVoltage(-12000);

	 }else{
		 angler.moveVoltage(0);
	 }
//------------------------------------------------------------------------------
if(DoubleTower.isPressed()){
intake.tarePosition();
intake.moveRelative(-750, 100);
pros::delay(500);

}
//------------------------------------------------------------------------------
	pros::delay(20);
 }
}

/**

Broken Ports:13,14

cd "C:\Users\bstar\git\2616J\2616J-TT1 - BLUE"
prosv5 make
prosv5 upload --slot 3 --name TTBlueStack
*/
