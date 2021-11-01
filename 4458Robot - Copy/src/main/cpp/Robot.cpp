// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.
// OLD NAME TankTeleop
#include <frc/Joystick.h>
#include <frc/PWMSparkMax.h>
#include <frc/Encoder.h>
#include <frc/TimedRobot.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>



#include <cameraserver/CameraServer.h>
#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc/imgproc.hpp>

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


  void spencerArm() {
      wpi::outs() << armEncoder.GetDistance();
      float armSpeed = 0;
      if(m_leftStick.GetRawButton(6)) {
        armSpeed = 1;
      } else if(m_leftStick.GetRawButton(4)) {  
        if(armEncoder.GetDistance() > 0) {
          armSpeed = -1;
        }
      }
      m_armMotor.Set(armSpeed);
      
      if(m_leftStick.GetRawButtonPressed(12)) {
        armEncoder.SetReverseDirection(true);
        armEncoder.Reset();
      }

      float hookSpeed = 0;
      if(m_rightStick.GetRawButton(7)) {
        hookSpeed = 1;
      } else if(m_rightStick.GetRawButton(8)) {
        hookSpeed = -1;
      }
      m_hookMotor.Set(hookSpeed);
    }

    void spencerShoot() {
      

      float ejectLeft = 0;
      float ejectRight = 0;
      float conveyor = 0;

      int rightTrigger = m_rightStick.GetTrigger();
      m_ejectMotorL1.Set(-rightTrigger);
      m_ejectMotorR1.Set(rightTrigger);

      int leftTrigger = m_leftStick.GetTrigger();
      m_intakeMotor.Set(-leftTrigger);
      if(leftTrigger || rightTrigger) conveyor = 1;

      if(m_rightStick.GetRawButton(3)) {
        ejectLeft = -0.25;
        ejectRight = 0.25;
      }
      if(m_rightStick.GetRawButton(4)) {
        ejectLeft = -0.5;
        ejectRight = 0.5;
      }
      if(m_rightStick.GetRawButton(2)) {
        ejectLeft = -1;
        ejectRight = 1;
      }
      if(m_leftStick.GetRawButton(11)) {
        conveyor = -1;
        m_ejectMotorL1.Set(1);
        m_ejectMotorR1.Set(-1);
        m_intakeMotor.Set(1);
      }
      if(m_leftStick.GetRawButtonReleased(11)) {
        conveyor = 0;
        m_ejectMotorL1.Set(0);
        m_ejectMotorR1.Set(0);
        m_intakeMotor.Set(0);
      }

      m_ejectMotorL0.Set(ejectLeft);
      m_ejectMotorR0.Set(ejectRight);
      m_conveyorMotor.Set(conveyor);
    }

  void TeleopPeriodic() override { 
      leftX = m_leftStick.GetX();
      leftY = m_leftStick.GetY();
      rightX = m_rightStick.GetX();
      rightY = m_rightStick.GetY();

      deadzone(&leftX, deadzoneValue);
      deadzone(&leftY, deadzoneValue);
      deadzone(&rightX, deadzoneValue);
      deadzone(&rightY, deadzoneValue);

      switch(mode) {
        // case 0:
        //   m_driveMotorL0.Set(-leftY);
        //   m_driveMotorL1.Set(-leftY);
        //   m_driveMotorR0.Set(rightY); 
        //   m_driveMotorR1.Set(rightY);
        //   break;
        case 1:
          if(rightX == 0) {
            // if we are tilted left, add a little to rightX
            // if we are tilted right, subtract a little from rightX

            // maybe like 0.05? 0.1?`

            // but how do we figure out if we are tiltled?
            // reset gyro every time we start driving straight?
          }
          m_driveMotorL0.Set(-leftY + (rightX * 2));
          m_driveMotorL1.Set(-leftY + (rightX * 2));
          m_driveMotorR0.Set(leftY - (-rightX * 2));
          m_driveMotorR1.Set(leftY - (-rightX * 2));
          spencerShoot();
          spencerArm();

          float turnSpeed = 0;
          if(m_rightStick.GetRawButton(9)) {
            turnSpeed = -1;
          } else if(m_rightStick.GetRawButton(10)) {
            turnSpeed = 1;
          }
          m_turnTable.Set(turnSpeed);
          break;
        // case 2:
        //   m_driveMotorL0.Set(-leftY + (leftX * 2));
        //   m_driveMotorL1.Set(-leftY + (leftX * 2));
        //   m_driveMotorR0.Set(leftY - (-leftX * 2));
        //   m_driveMotorR1.Set(leftY - (-leftX * 2));
        //   break;
      }

      // profiles
      if(m_leftStick.GetRawButtonPressed(7)) mode = 0;
      if(m_leftStick.GetRawButtonPressed(8)) mode = 1;
      if(m_leftStick.GetRawButtonPressed(9)) mode = 2; // spencer
      if(m_leftStick.GetRawButtonPressed(10)) mode = 3;
    }

    void RobotInit() override {
      // std::thread visionThread(VisionThread);
      // visionThread.detach();
      armEncoder.SetReverseDirection(true);
    }

    void AutonomousInit() override {
      m_ejectMotorL0.Set(-1);
      m_ejectMotorR0.Set(1);
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      m_ejectMotorL1.Set(-1);
      m_ejectMotorR1.Set(1);
      m_conveyorMotor.Set(1);

      std::this_thread::sleep_for(std::chrono::milliseconds(3000));
      m_ejectMotorL1.Set(0);
      m_ejectMotorR1.Set(0);
      m_conveyorMotor.Set(0);
      m_ejectMotorL0.Set(0);
      m_ejectMotorR0.Set(0);


      m_driveMotorL0.Set(1);
      m_driveMotorR0.Set(1);
      m_driveMotorL1.Set(1);
      m_driveMotorR1.Set(1);
      std::this_thread::sleep_for(std::chrono::milliseconds(1500));
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

  frc::PWMSparkMax m_turnTable{18};

  frc::Encoder armEncoder{4, 5};

};

#ifndef RUNNING_FRC_TESTS
int main() {
  return frc::StartRobot<Robot>();
}
#endif
