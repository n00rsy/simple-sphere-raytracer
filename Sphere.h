#pragma once
#include "Object.h"
class Sphere : public Object {
public:
    float radius, radius2;

    Sphere(
        const Vec3& p,
        const float& r,
        const Vec3& sc,
        const float& refl = 0,
        const float& transp = 0,
        const Vec3& ec = 0) :
        radius(r), radius2(r* r), Object(p, sc, refl, transp, ec)
    {
    }

    bool intersect(const Vec3& rayorig, const Vec3& raydir, float& t0, float& t1) const {
        Vec3 l = position - rayorig;
        float tca = l.dotProduct(raydir);
        if (tca < 0) return false;
        float d2 = l.dotProduct(l) - tca * tca;
        if (d2 > radius2) return false;
        float thc = sqrt(radius2 - d2);
        t0 = tca - thc;
        t1 = tca + thc;

        return true;
    }

    void test() {
        cout << "child class" << endl;
    }
};