#include <frc/PWMSparkMax.h>
#include <frc/Encoder.h>

#ifndef SWERVE_H
#define SWERVE_H

class Swerve {
    public:
        Swerve(double l, double w);
        void Drive(double y1, double x2, double y2);
        double GetAngle(frc::Encoder encoder);
        void SetAngle(double angle, frc::PWMSparkMax motor, frc::Encoder encoder);
        void ResetAngle();
    private:
        // static void RotateThread(double angle, frc::PWMSparkMax motor, frc::Encoder encoder, Swerve *translate);
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

        double l, w, A, B, C, D, R;
};
#endif