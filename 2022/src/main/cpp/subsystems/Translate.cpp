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
    return frontLeftEncoder.GetDistance();
}

void Translate::SetAngle(double angle) { // 1 right, -1 left
    std::thread thread(RotateThread, angle);
    thread.detach();
}

void Translate::RotateThread(double angle) {
    // im assuming encoders go from 0 to 360 and not -180 to 180?
    // this wont account for wrapping, implement that later
    double power = 0;

    if(Translate::GetAngle() < angle) power = -1;
    else power = 1;

    while(frontLeftEncoder.GetDistance() - 0.5 > GetAngle() && frontLeftEncoder.GetDistance() + 0.5 < GetAngle()) {
        frontLeftRotate.Set(1);
        frontRightRotate.Set(1);
        backLeftRotate.Set(1);
        backRightRotate.Set(1);
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