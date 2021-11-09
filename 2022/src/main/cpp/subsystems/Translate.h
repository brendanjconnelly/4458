#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <frc/PWMSparkMax.h>
#include <frc/Encoder.h>
class Translate {
    public:
        Translate();
        void Drive(double power, double angle);
        double GetAngle();
        void SetAngle(double angle, Translate *translate);
        void ResetAngle();
    private:
        static void RotateThread(double angle, Translate *translate);
        bool killThread;

        frc::PWMSparkMax frontLeft;
        frc::PWMSparkMax frontRight;
        frc::PWMSparkMax backLeft;
        frc::PWMSparkMax backRight;

        frc::PWMSparkMax frontLeftRotate;
        frc::PWMSparkMax frontRightRotate;
        frc::PWMSparkMax backLeftRotate;
        frc::PWMSparkMax backRightRotate;

        frc::Encoder frontLeftEncoder;
        frc::Encoder frontRightEncoder;
        frc::Encoder backLeftEncoder;
        frc::Encoder backRightEncoder;
};
#endif