// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Robot.h"

#include <iostream>
#include <math.h>

#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/commands/Command.h>
#include <frc/Joystick.h>

#include "subsystems/Translate.h"
#include "subsystems/Rotate.h"
#include "MathHelper.h"
class Robot : public frc::TimedRobot {
  void RobotInit() {
    // m_chooser.SetDefaultOption("Drive Forward", driveForward);
    // m_chooser.AddOption("Complex Auto", complexAuto);
    // frc::SmartDashboard::PutData("Auto Modes", &m_chooser);
  }

  /**
   * This function is called every robot packet, no matter the mode. Use
   * this for items like diagnostics that you want ran during disabled,
   * autonomous, teleoperated and test.
   *
   * <p> This runs after the mode specific periodic functions, but before
   * LiveWindow and SmartDashboard integrated updating.
   */
  void RobotPeriodic() {}

  /**
   * This autonomous (along with the chooser code above) shows how to select
   * between different autonomous modes using the dashboard. The sendable chooser
   * code works with the Java SmartDashboard. If you prefer the LabVIEW Dashboard,
   * remove all of the chooser code and uncomment the GetString line to get the
   * auto name from the text box below the Gyro.
   *
   * You can add additional auto modes by adding additional comparisons to the
   * if-else structure below with additional strings. If using the SendableChooser
   * make sure to add them to the chooser code above as well.
   */
  void AutonomousInit() {
    // m_autoSelected = m_chooser.GetSelected();
    // // m_autoSelected = SmartDashboard::GetString("Auto Selector",
    // //     kAutoNameDefault);
    // std::cout << "Auto selected: " << m_autoSelected << std::endl;

    // if (m_autoSelected == kAutoNameCustom) {
    //   // Custom Auto goes here
    // } else {
    //   // Default Auto goes here
    // }
  }

  void Robot::AutonomousPeriodic() {
    // if (m_autoSelected == kAutoNameCustom) {
    //   // Custom Auto goes here
    // } else {
    //   // Default Auto goes here
    // }
  }

  void TeleopInit() {
    
  }

  void TeleopPeriodic() {
    m_translate.Drive(pythagorean(leftStick.GetX(), leftStick.GetY()), leftStick.GetDirectionDegrees());
    m_rotate.Turn(rightStick.GetX());
  }

  void DisabledInit() {}

  void DisabledPeriodic() {}

  void TestInit() {}

  void TestPeriodic() {}
  
  private:
    frc::Joystick leftStick{0};
    frc::Joystick rightStick{1};
    Translate m_translate;
    Rotate m_rotate;
    // frc::SendableChooser<frc2::Command*> m_chooser;
};

#ifndef RUNNING_FRC_TESTS
int main() {
  return frc::StartRobot<Robot>();
}
#endif
