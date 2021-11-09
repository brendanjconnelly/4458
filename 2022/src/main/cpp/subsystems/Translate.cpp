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

    frontLeft.SetInverted(true);
    backLeft.SetInverted(true);

    frontLeftRotate.SetInverted(true);
    backLeftRotate.SetInverted(true);
    frontLeftEncoder.SetReverseDirection(true);
    backLeftEncoder.SetReverseDirection(true);
}

void Translate::Drive(double power, double angle) { // 1 forward, -1 backward
    SetAngle(angle, this);
    frontLeft.Set(power);
    frontRight.Set(power);
    backLeft.Set(power);
    frontRight.Set(power);
}

double Translate::GetAngle() {
    return (frontLeftEncoder.GetDistance() + frontRightEncoder.GetDistance() + backLeftEncoder.GetDistance() + backRightEncoder.GetDistance()) / 4; // average of the 4 encoders
}

void Translate::SetAngle(double angle, Translate *translate) { // 1 right, -1 left
    killThread = true;
    std::thread rotateThread(RotateThread, angle, this);
    rotateThread.detach();
}

void Translate::RotateThread(double angle, Translate *translate) {
    // im assuming encoders go from 0 to 360 and not -180 to 180?
    // this wont account for wrapping, implement that later
    double power = 0;
    double precision = 0.5; // how many degrees in either way can the wheels face to be considered accurate

    // this bit just sets the power so the wheels go in the optimal direction
    if((*translate).GetAngle() < angle) power = 1;
    else power = -1;

    if(abs((*translate).GetAngle() - angle) > 180) {
        power *= -1;
    }

    // for the love of god rewrite his line please I just don't feel like it right now
    while((*translate).frontLeftEncoder.GetDistance() - precision > (*translate).GetAngle() && (*translate).frontLeftEncoder.GetDistance() + precision < (*translate).GetAngle()
    && (*translate).frontRightEncoder.GetDistance() - precision > (*translate).GetAngle() && (*translate).frontRightEncoder.GetDistance() + precision < (*translate).GetAngle()
    && (*translate).backLeftEncoder.GetDistance() - precision > (*translate).GetAngle() && (*translate).backLeftEncoder.GetDistance() + precision < (*translate).GetAngle()
    && (*translate).backRightEncoder.GetDistance() - precision > (*translate).GetAngle() && (*translate).backRightEncoder.GetDistance() + precision < (*translate).GetAngle()) {
        if((*translate).frontLeftEncoder.GetDistance() - precision > (*translate).GetAngle() && (*translate).frontLeftEncoder.GetDistance() + precision < (*translate).GetAngle())
        (*translate).frontLeftRotate.Set(power);
        else (*translate).frontLeftRotate.Set(0);
        if((*translate).frontRightEncoder.GetDistance() - precision > (*translate).GetAngle() && (*translate).frontRightEncoder.GetDistance() + precision < (*translate).GetAngle())
        (*translate).frontRightRotate.Set(power);
        else (*translate).frontRightRotate.Set(0);
        if((*translate).backLeftEncoder.GetDistance() - precision > (*translate).GetAngle() && (*translate).backLeftEncoder.GetDistance() + precision < (*translate).GetAngle())
        (*translate).backLeftRotate.Set(power);
        else (*translate).backLeftRotate.Set(0);
        if((*translate).backRightEncoder.GetDistance() - precision > (*translate).GetAngle() && (*translate).backRightEncoder.GetDistance() + precision < (*translate).GetAngle())
        (*translate).backRightRotate.Set(power);
        else (*translate).backRightRotate.Set(0);
    }

}

void Translate::ResetAngle() {
    frontLeftEncoder.Reset();
    frontRightEncoder.Reset();
    backLeftEncoder.Reset();
    backLeftEncoder.Reset();
}

