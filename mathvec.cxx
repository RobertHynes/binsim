/*
  Class to hold mathematical vector

  Released as part of BinSim 1.0
  This release built on August 25 2025

  Author: Robert I. Hynes (rhynes@lsu.edu)
          Louisiana State University
          Department of Physics and Astronomy
          Baton Rouge
          Louisiana, USA

  Copyright (C) 2025 Robert I. Hynes

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>. 
*/

#include <cmath>

#include "mathvec.h"

/*****************************************************************************/
// Define operations on 3D vectors

// Modulus of vector
float Vec3::mod()
{
  return sqrt(x*x + y*y + z*z);
}

// Normalize vector
void Vec3::normalize()
{
  const float a = mod();
  if (a) {
    x /= a;
    y /= a;
    z /= a;
  }
}

// Add vector to this 
Vec3& Vec3::operator+= (const Vec3 a)
{
  x += a.x;
  y += a.y;
  z += a.z;
  return *this;
}

// Subtract vector from this
Vec3& Vec3::operator-= (const Vec3 a)
{
  x -= a.x;
  y -= a.y;
  z -= a.z;
  return *this;
}

// Multiply by scalar
Vec3& Vec3::operator*= (const float a)
{
  x *= a;
  y *= a;
  z *= a;
  return *this;
}

// Divide by scalar
Vec3& Vec3::operator/= (const float a) 
{ 
  x /= a; 
  y /= a; 
  z /= a; 
  return *this; 
}

// Vector addition and subtraction
Vec3 operator+ (const Vec3 a, const Vec3 b)  { Vec3 c = a ; return c += b; }
Vec3 operator- (const Vec3 a, const Vec3 b)  { Vec3 c = a ; return c -= b; }

// Multiplication and division by a scalar
Vec3 operator* (const Vec3 a, const float b) { Vec3 c = a ; return c *= b; }
Vec3 operator/ (const Vec3 a, const float b) { Vec3 c = a ; return c /= b; }

// Scalar product
float operator* (const Vec3 a, const Vec3 b)
{
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Vector product
Vec3 operator% (const Vec3 a, const Vec3 b)
{
  return Vec3(a.y * b.z - a.z * b.y, 
	      a.z * b.x - a.x * b.z, 
	      a.x * b.y - a.y * b.x);
}

// Output
ostream& operator<< (ostream &s, const Vec3 a)
{
  return s << '{' << a.x << ',' << a.y << ',' << a.z << '}';
}
