#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <vector>
#include <iostream>
#include <cassert>
#include <algorithm>

#include "Vector3.h"
#include "Object.h"
#include "Sphere.h"

#define M_PI 3.141592653589793
#define INFINITY 1e8
#define MAX_TRACE_DEPTH 5 //max recursion



using namespace std;

float mix(const float& a, const float& b, const float& mix) {
    return b * mix + a * (1 - mix);
}

Vec3 trace(
    const Vec3& rayorig,
    const Vec3& raydir,
    const vector<Sphere>& spheres,
    const int& depth) {
    float tnear = INFINITY;
    const Object* sphere = NULL;
    // find intersection of this ray with the sphere in the scene
    for (unsigned i = 0; i < spheres.size(); ++i) {
        float t0 = INFINITY, t1 = INFINITY;
        if (spheres[i].intersect(rayorig, raydir, t0, t1)) {
            if (t0 < 0) t0 = t1;
            if (t0 < tnear) {
                tnear = t0;
                sphere = &spheres[i];
            }
        }
    }
    // if there's no intersection return background color
    if (!sphere) return Vec3(2);

    // here we goooo
    Vec3 surfaceColor = 0;
    Vec3 phit = rayorig + raydir * tnear; // point of intersection
    Vec3 nhit = phit - sphere->position; // normal at the intersection point
    nhit.normalize();
    float bias = 1e-4;
    bool inside = false;
    if (raydir.dotProduct(nhit) > 0) nhit = -nhit, inside = true;
    if ((sphere->transparency > 0 || sphere->reflection > 0) && depth < MAX_TRACE_DEPTH) {
        float facingratio = -raydir.dotProduct(nhit);
        float fresneleffect = mix(pow(1 - facingratio, 3), 1, 0.1);

        // compute reflection direction 
        Vec3 refldir = raydir - nhit * 2 * raydir.dotProduct(nhit);
        refldir.normalize();
        Vec3 reflection = trace(phit + nhit * bias, refldir, spheres, depth + 1);
        Vec3 refraction = 0;
        if (sphere->transparency) {
            float ior = 1.1, eta = (inside) ? ior : 1 / ior; // inside or outside the surface
            float cosi = -nhit.dotProduct(raydir);
            float k = 1 - eta * eta * (1 - cosi * cosi);
            Vec3 refrdir = raydir * eta + nhit * (eta * cosi - sqrt(k));
            refrdir.normalize();
            refraction = trace(phit - nhit * bias, refrdir, spheres, depth + 1);
        }
        surfaceColor = (
            reflection * fresneleffect +
            refraction * (1 - fresneleffect) * sphere->transparency) * sphere->surfaceColor;
    }
    else {
        // diffuse
        for (int i = 0; i < spheres.size(); ++i) {
            if (spheres[i].emissionColor.x > 0) {
                // light
                Vec3 transmission = 1;
                Vec3 lightDirection = spheres[i].position - phit;
                lightDirection.normalize();
                for (unsigned j = 0; j < spheres.size(); ++j) {
                    if (i != j) {
                        float t0, t1;
                        if (spheres[j].intersect(phit + nhit * bias, lightDirection, t0, t1)) {
                            transmission = 0;
                            break;
                        }
                    }
                }
                surfaceColor += sphere->surfaceColor * transmission *
                    max(float(0), nhit.dotProduct(lightDirection)) * spheres[i].emissionColor;
            }
        }
    }
    return surfaceColor + sphere->emissionColor;
}

void render(const vector<Sphere>& spheres) {
    unsigned width = 640, height = 480;
    Vec3* image = new Vec3[width * height], * pixel = image;
    float invWidth = 1 / float(width), invHeight = 1 / float(height);
    float fov = 50, aspectratio = width / float(height);
    float angle = tan(M_PI * 0.5 * fov / 180.);
    // Trace rays
    for (unsigned y = 0; y < height; ++y) {
        for (unsigned x = 0; x < width; ++x, ++pixel) {
            float xx = (2 * ((x + 0.5) * invWidth) - 1) * angle * aspectratio;
            float yy = (1 - 2 * ((y + 0.5) * invHeight)) * angle;
            Vec3 raydir(xx, yy, -1);
            raydir.normalize();
            *pixel = trace(Vec3(0), raydir, spheres, 0);
        }
    }
    // Save result
    ofstream ofs("./render.ppm", ios::out | ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (unsigned i = 0; i < width * height; ++i) {
        ofs << (unsigned char)(min(float(1), image[i].x) * 255) <<
            (unsigned char)(min(float(1), image[i].y) * 255) <<
            (unsigned char)(min(float(1), image[i].z) * 255);
    }
    ofs.close();
    delete[] image;
}

int main(int argc, char** argv) {

    //set up scene
    vector<Sphere> spheres;
    // position, radius, surface color, reflectivity, transparency, emission color
    spheres.push_back(Sphere(Vec3(0.0, -10004, -20), 10000, Vec3(0.20, 0.20, 0.20), 0, 0.0));
    spheres.push_back(Sphere(Vec3(0.0, 0, -30), 4, Vec3(1.00, 0.32, 0.36), 0.2, 0));
    spheres.push_back(Sphere(Vec3(-10, 3, -30), 6, Vec3(1.00, 0.2, 0.5), 0.2, 0));
    spheres.push_back(Sphere(Vec3(5.0, -1, -15), 2, Vec3(0.90, 0.76, 0.46), 0.1, 0.0));
    spheres.push_back(Sphere(Vec3(5.0, 0, -25), 3, Vec3(0.65, 0.77, 0.97), 0.3, 0.0));

    // light
    spheres.push_back(Sphere(Vec3(0.0, 20, -20), 3, Vec3(0.00, 0.00, 0.00), 0, 0.0, Vec3(3,3,3)));
    render(spheres);
    return 0;
}