#pragma once

#include "Vector3.h"

class Object {
public:
    Vec3 position, rotation;
    Vec3 surfaceColor, emissionColor;
    float transparency, reflection;
    Object(
        const Vec3& p,
        const Vec3& sc,
        const float& refl = 0,
        const float& transp = 0,
        const Vec3& ec = 0) :
        position(p), surfaceColor(sc), emissionColor(ec),
        transparency(transp), reflection(refl)
    {
    }
    void test() {
        cout << "parent class" << endl;
    }
};
