#ifndef ROTATE_H
#define ROTATE_H

#include <frc/PWMSparkMax.h>
#include <frc/Encoder.h>

class Rotate {
    public:
        Rotate();
        void Turn(double direction);
        double GetAngle();
        void SetAngle(double angle);

    private:
        void RotateThread(double angle);
        frc::PWMSparkMax frontLeft;
        frc::PWMSparkMax frontRight;
        frc::PWMSparkMax backLeft;
        frc::PWMSparkMax backRight;
};

#endif