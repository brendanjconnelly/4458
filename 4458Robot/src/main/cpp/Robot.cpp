// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include <frc/Joystick.h>
#include <frc/PWMSparkMax.h>
#include <frc/Encoder.h>
#include <frc/TimedRobot.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>

// #include <cameraserver/CameraServer.h>
// #include <opencv2/core/core.hpp>
// #include <opencv2/core/types.hpp>
// #include <opencv2/imgproc/imgproc.hpp>

#include "Constants.h"

void deadzone(float *stick, float deadzoneValue) {
  if(abs(*stick) < deadzoneValue) {
    *stick = 0;
  } else {
    *stick += (deadzoneValue * std::ceil(*stick)); // weird line but it sets the sign of the deadzone to the same sign as the stick position and adds it, so basically -deadzone < x < deaadzone = 0, -deadzone and deadzone = 0. deadzone+.1 = 0.1
  }
}

void coast(float *stick, float newPosition, float coastInc) {

  if(*stick == 0) {
    *stick += (coastInc * std::ceil(newPosition));
  } else {
    *stick += (coastInc * std::ceil(newPosition)); // similar to above
  }
}

class Robot : public frc::TimedRobot {
 public:

  int mode = 1 ;
  float deadzoneValue = 0.05f;
  
  float coastInc = 0.1f;

  float leftX = 0;
  float leftY = 0;
  float rightX = 0;
  float rightY = 0;

  float driveL0 = 0;
  float driveL1 = 0;
  float driveR0 = 0;
  float driveR1 = 0;

  float ejectL0 = 0;
  float ejectL1 = 0;
  float ejectR0 = 0;
  float ejectR1 = 0;

  float intake = 0;
  float conveyor = 0;

  float arm = 0;

  float turnTable = 0;
  float hook = 0;


  void spencerArm() {
    if(m_leftStick.GetRawButton(6)) {
      arm = 1;
    } else if(m_leftStick.GetRawButton(4)) {  
      if(armEncoder.GetDistance() > 0) {
        arm = -1;
      }
    }
    
    if(m_leftStick.GetRawButtonPressed(12)) {
      armEncoder.Reset();
    }

    if(m_rightStick.GetRawButton(7)) {
      hook = 1;
    } else if(m_rightStick.GetRawButton(8)) {
      hook = -1;
    }
  }

   void spencerShoot() {
    int rightTrigger = m_rightStick.GetTrigger();
    ejectL1 = rightTrigger;
    ejectR1 = rightTrigger;

    int leftTrigger = m_leftStick.GetTrigger();
    intake = leftTrigger;
    if(leftTrigger || rightTrigger) conveyor = 1;

    if(m_rightStick.GetRawButton(2)) {
      ejectL0 = 1;
      ejectR0 = 1;
    }
    if(m_leftStick.GetRawButton(11)) {
      conveyor = -1;
      ejectL1 = -1;
      ejectR1 = -1;
      intake = -1;
    }
  }

  static void turnThread(float *turn, int degrees) {
    std::chrono::milliseconds time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    std::chrono::milliseconds now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    while(time.count() + 725*(degrees/360*8) > now.count()) {
      (*turn) = -0.25;
      now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    }
  }

  void TeleopPeriodic() override { 
      leftX = m_leftStick.GetX();
      leftY = m_leftStick.GetY();
      rightX = m_rightStick.GetX();
      rightY = m_rightStick.GetY();

      // so basically each motor has a variable that is changed and "committed"
      // to the motor at the end of each loop

      // if you dont want to override any previous changes to the value,
      // then you simply check if the value is 0, and if it is then
      // nobody has touched it yet

      {
      driveL0 = 0;
      driveR0 = 0;
      driveL1 = 0;
      driveR1 = 0;

      ejectL0 = 0;
      ejectR0 = 0;
      ejectL1 = 0;
      ejectR1 = 0;

      arm = 0;

      intake = 0;
      conveyor = 0;
      
      turnTable = 0;
      hook = 0;
      }

      deadzone(&leftX, deadzoneValue);
      deadzone(&leftY, deadzoneValue);
      deadzone(&rightX, deadzoneValue);
      deadzone(&rightY, deadzoneValue);

      switch(mode) {
        case 0:
          m_driveMotorL0.Set(-leftY);
          m_driveMotorL1.Set(-leftY);
          m_driveMotorR0.Set(rightY); 
          m_driveMotorR1.Set(rightY);
          break;
        case 1: // this is the default profile
          if(rightX == 0) {} // figure the gyro out (if we get one)
          driveL0 = leftY - (rightX * 2);
          driveR0 = leftY -  (-rightX * 2);
          driveL1 = leftY - (rightX * 2);
          driveR1 = leftY - (-rightX * 2);
          spencerShoot();
          spencerArm();

          if(m_rightStick.GetRawButtonPressed(5)) {
            std::thread thread(turnThread, &turnTable, 1440);
            thread.detach();
          } 
          if(m_rightStick.GetRawButtonPressed(9)) { // 720
            std::thread thread(turnThread, &turnTable, 720);
            thread.detach();
          }
          if(m_rightStick.GetRawButtonPressed(10)) { // 765
            std::thread thread(turnThread, &turnTable, 765);
            thread.detach();
          }
          if(m_rightStick.GetRawButtonPressed(11)) { // 810
            std::thread thread(turnThread, &turnTable, 810);
            thread.detach();
          }
          if(m_rightStick.GetRawButtonPressed(12)) { // 855
            std::thread thread(turnThread, &turnTable, 855);
            thread.detach();
          }

          if(m_rightStick.GetRawButton(6)) {
            turnTable = -0.25;
          }
          if(m_rightStick.GetRawButton(4)) {
            turnTable = 0.25;
          }

          break;
        case 2:
          m_driveMotorL0.Set(-leftY + (leftX * 2));
          m_driveMotorL1.Set(-leftY + (leftX * 2));
          m_driveMotorR0.Set(leftY - (-leftX * 2));
          m_driveMotorR1.Set(leftY - (-leftX * 2));
          break;
      }

      // profiles
      if(m_leftStick.GetRawButtonPressed(7)) mode = 0;
      if(m_leftStick.GetRawButtonPressed(8)) mode = 1;
      if(m_leftStick.GetRawButtonPressed(9)) mode = 2; // spencer
      if(m_leftStick.GetRawButtonPressed(10)) mode = 3;

      {
        m_driveMotorL0.Set(driveL0);
        m_driveMotorR0.Set(driveR0);
        m_driveMotorL1.Set(driveL1);
        m_driveMotorR1.Set(driveR1);

        m_ejectMotorL0.Set(ejectL0);
        m_ejectMotorL1.Set(ejectL1);
        m_ejectMotorR0.Set(ejectR0);
        m_ejectMotorR1.Set(ejectR1);

        m_intakeMotor.Set(intake);
        m_conveyorMotor.Set(conveyor);

        m_turnTableMotor.Set(turnTable);
        m_hookMotor.Set(hook);

        m_armMotor.Set(arm);
      }
    }

    void RobotInit() override {
      // std::thread visionThread(VisionThread);
      // visionThread.\detach();
      armEncoder.SetReverseDirection(true);
      m_driveMotorL0.SetInverted(true);
      m_driveMotorL1.SetInverted(true);

      m_intakeMotor.SetInverted(true);
      m_ejectMotorL0.SetInverted(true);
      m_ejectMotorL1.SetInverted(true);
    }

    void AutonomousInit() override {

      // some good programming here

      m_ejectMotorL0.Set(1);
      m_ejectMotorR0.Set(1);
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      m_ejectMotorL1.Set(1);
      m_ejectMotorR1.Set(1);
      m_conveyorMotor.Set(1);

      std::this_thread::sleep_for(std::chrono::milliseconds(3000));
      m_ejectMotorL1.Set(0);
      m_ejectMotorR1.Set(0);
      m_conveyorMotor.Set(0);
      m_ejectMotorL0.Set(0);
      m_ejectMotorR0.Set(0);


      m_driveMotorL0.Set(0.5);
      m_driveMotorR0.Set(0.5);
      m_driveMotorL1.Set(0.5);
      m_driveMotorR1.Set(0.5);
      std::this_thread::sleep_for(std::chrono::milliseconds(4000));
      m_driveMotorL0.Set(0);
      m_driveMotorR0.Set(0);
      m_driveMotorL1.Set(0);
      m_driveMotorR1.Set(0);
    }

 private:
  frc::Joystick m_leftStick{0};
  frc::Joystick m_rightStick{1};

  frc::PWMSparkMax m_driveMotorL0{2};
  frc::PWMSparkMax m_driveMotorL1{1};
  frc::PWMSparkMax m_driveMotorR0{4};
  frc::PWMSparkMax m_driveMotorR1{3};

  frc::PWMSparkMax m_intakeMotor{5};
  frc::PWMSparkMax m_conveyorMotor{19};


  frc::PWMSparkMax m_ejectMotorL0{6};
  frc::PWMSparkMax m_ejectMotorL1{8};
  frc::PWMSparkMax m_ejectMotorR0{7};
  frc::PWMSparkMax m_ejectMotorR1{9};

  frc::PWMSparkMax m_armMotor{17};
  frc::PWMSparkMax m_hookMotor{0};

  frc::PWMSparkMax m_turnTableMotor{18};

  frc::Encoder armEncoder{4, 5};


};

#ifndef RUNNING_FRC_TESTS
int main() {
  return frc::StartRobot<Robot>();
}
#endif
