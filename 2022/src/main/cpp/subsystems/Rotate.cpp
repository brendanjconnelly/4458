#include "Rotate.h"
#include "../Constants.h"

Rotate::Rotate():
    frontLeft{FRONT_LEFT},
    frontRight{FRONT_RIGHT},
    backLeft{BACK_LEFT},
    backRight{BACK_RIGHT} {

}

void Rotate::Turn(double direction) { // -1 is left, +1 is right
    frontLeft.Set(frontLeft.Get() + 2 * direction);
    backLeft.Set(backLeft.Get() + 2 * direction);

    frontRight.Set(frontRight.Get() - 2 * direction);
    backRight.Set(backRight.Get() - 2 * direction);
}

double Rotate::GetAngle() {

}