#include "Swerve.h"
#include "../Constants.h"
#include "../MathHelper.h"



Swerve::Swerve(double l, double w) :
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

    Swerve::l = l;
    Swerve::w = w;

    frontLeft.SetInverted(true);
    backLeft.SetInverted(true);

    frontLeftRotate.SetInverted(true);
    backLeftRotate.SetInverted(true);
    frontLeftEncoder.SetReverseDirection(true);
    backLeftEncoder.SetReverseDirection(true);
}

void Swerve::Drive(double x1, double y1, double x2) { // 1 forward, -1 backward
    R = pythagorean(l, w);
    A = x1 - x2*(l/R); 
    B = x1 + x2*(l/R); 
    C = y1 - x2*(w/R); 
    D = y1 + x2*(w/R); 

    frontRight.Set(pythagorean(B, C));
    backRight.Set(pythagorean(B, D));
    backLeft.Set(pythagorean(A, D));
    frontLeft.Set(pythagorean(A, C));

    frontRightRotate.
        
    // mecanum
    // x: pythagorean(Ax, Bx) * cos((theta+jsa)/psi)
    // x: pythagorean(Ax, Bx) * sin((theta+jsa)/psi)
}

double Swerve::GetAngle(frc::Encoder encoder) {
    return encoder.GetDistance();
}

void Swerve::SetAngle(double angle, frc::PWMSparkMax motor, frc::Encoder encoder) { // 1 right, -1 left
    double power = 0;
    double precision = 0.5; // how many degrees in either way can the wheels face to be considered accurate
    // this bit just sets the power so the wheels go in the optimal direction
    if((GetAngle(encoder) < angle)) power = 1;
    else power = -1;

    if(abs(GetAngle(encoder)).GetAngle() - angle) > 180) {
        power *= -1;
    }
}

void Translate::ResetAngle() {
    frontLeftEncoder.Reset();
    frontRightEncoder.Reset();
    backLeftEncoder.Reset();
    backLeftEncoder.Reset();
}

