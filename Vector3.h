#pragma once

#include "Vector3.h"
#include <iostream>

using namespace std;

template <typename T>
class Vector3 {
public:
	T x, y, z;
	//constructors
	Vector3() {
		x = 0;
		y = 0;
		z = 0;
	}
	Vector3(T mag) {
		x = mag;
		y = mag;
		z = mag;
	}
	Vector3(T xx, T yy, T zz) {
		x = xx;
		y = yy;
		z = zz;
	}

	//operator overrides
	Vector3 <T> operator * (const T& s) const {
		return Vector3<T>(x * s, y * s, z * s);
	}
	Vector3<T> operator * (const Vector3<T>& v) const {
		return Vector3<T>(x * v.x, y * v.y, z * v.z);
	}
	Vector3<T> operator - (const Vector3<T>& v)const {
		return Vector3<T>(x - v.x, y - v.y, z - v.z);
	}
	Vector3<T> operator + (const Vector3<T>& v)const {
		return Vector3<T>(x + v.x, y + v.y, z + v.z);
	}
	Vector3<T>& operator += (const Vector3<T>& v) {
		x += v.x, y += v.y, z += v.z;
		return *this;
	}
	Vector3<T>& operator *= (const Vector3<T>& v) {
		x *= v.x, y *= v.y, z *= v.z;
		return *this;
	}
	Vector3<T> operator - () const {
		return Vector3<T>(-x, -y, -z);
	}

	//other operations
	Vector3& normalize()
	{
		T nor2 = length2();
		if (nor2 > 0) {
			T invNor = 1 / sqrt(nor2);
			x *= invNor, y *= invNor, z *= invNor;
		}
		return *this;
	}
	T dotProduct(const Vector3<T>& v)const {
		return x * v.x + y * v.y + z * v.z;
	}
	T length2() const {
		return x * x + y * y + z * z;
	}
	T length()const {
		return sqrt(length2());
	}

	void printVector() const {
		cout << "x: " << x << " y: " << y << " z: " << z << " Len: " << this->length() << endl;
	}

};
typedef Vector3<float> Vec3;