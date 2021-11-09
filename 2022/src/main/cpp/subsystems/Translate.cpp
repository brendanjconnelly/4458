#include "Translate.h"
#include "../Constants.h"


Translate::Translate() :
    frontLeft{FRONT_LEFT},
    frontRight{FRONT_RIGHT},
    backLeft{BACK_LEFT},
    backRight{BACK_RIGHT},
    frontLeftRotate{FRONT_LEFT_ROTATE},
    frontRightRotate{FRONT_RIGHT_ROTATE},
    backLeftRotate{BACK_LEFT_ROTATE},
    backRightRotate{BACK_RIGHT_ROTATE},
    
    frontLeftEncoder{FRONT_LEFT_ENCODER0, FRONT_LEFT_ENCODER1},
    frontRightEncoder{FRONT_RIGHT_ENCODER0, FRONT_RIGHT_ENCODER1},
    backLeftEncoder{BACK_LEFT_ENCODER0, BACK_LEFT_ENCODER1},
    backRightEncoder{BACK_RIGHT_ENCODER0, BACK_RIGHT_ENCODER1} { // holy this is a lot of variables
    // set encoder distanceperpulse
    // note that these are *relative* encoders
}

void Translate::Drive(double power, double angle) { // 1 forward, -1 backward
    SetAngle(angle);
    frontLeft.Set(power);
    frontRight.Set(power);
    backLeft.Set(power);
    frontRight.Set(power);
}

double Translate::GetAngle() {
    return (frontLeftEncoder.GetDistance() + frontRightEncoder.GetDistance() + backLeftEncoder.GetDistance() + backRightEncoder.GetDistance()) / 4;
}

void Translate::SetAngle(double angle) { // 1 right, -1 left
    killThread = true;
    std::thread rotateThread(RotateThread, angle);
    rotateThread.detach();
}

void Translate::RotateThread(double angle) {
    // im assuming encoders go from 0 to 360 and not -180 to 180?
    // this wont account for wrapping, implement that later
    killThread = false;
    double power = 0;

    if(Translate::GetAngle() < angle) power = -1;
    else power = 1;
    // while(frontLeftEncoder.GetDistance() - 0.5 > GetAngle() && frontLeftEncoder.GetDistance() + 0.5 < GetAngle()) {
    while(true) {
        if(frontLeftEncoder.GetDistance() - 0.5 > GetAngle() && frontLeftEncoder.GetDistance() + 0.5 < GetAngle())
        frontLeftRotate.Set(power);
        if(frontRightEncoder.GetDistance() - 0.5 > GetAngle() && frontRightEncoder.GetDistance() + 0.5 < GetAngle())
        frontRightRotate.Set(power);
        if(backLeftEncoder.GetDistance() - 0.5 > GetAngle() && backLeftEncoder.GetDistance() + 0.5 < GetAngle())
        backLeftRotate.Set(power);
        if(backRightEncoder.GetDistance() - 0.5 > GetAngle() && backRightEncoder.GetDistance() + 0.5 < GetAngle())
        backRightRotate.Set(power);
        if(killThread) break;
    }
    frontLeftRotate.Set(0);
    frontRightRotate.Set(0);
    backLeftRotate.Set(0);
    backRightRotate.Set(0);
}

void Translate::ResetAngle() {
    frontLeftEncoder.Reset();
    frontRightEncoder.Reset();
    backLeftEncoder.Reset();
    backLeftEncoder.Reset();
}

