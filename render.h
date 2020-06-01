#pragma once

#include "Vector3.h"
#include <cstdlib>
#include <cstdio>

using namespace std;

float mix(const float& a, const float& b, const float& mix) {
    return b * mix + a * (1 - mix);
}

Vec3 trace(
    const Vec3& rayorig,
    const Vec3& raydir,
    const vector<Sphere>& spheres,
    const int& depth);

void render(const vector<Sphere>& spheres);