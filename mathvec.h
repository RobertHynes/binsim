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

#ifndef _VECLIB_H
#define _VECLIB_H

#include <iostream>

#include "binsim_stdinc.h"

using std::ostream;

/*****************************************************************************/

/*
  Structure to hold a 3-element float vector, with standard
  mathematical operations defined
*/
struct Vec3 
{
  // Members
  float x, y, z;

  // Constructors
  Vec3() : x(0.0f), y(0.0f), z(0.0f) { }

  Vec3(const float x1, const float y1, const float z1 = 0.0f) : 
    x(x1), y(y1), z(z1) { }

  // Single vector functions
  float mod();
  void normalize();

  // Self-modifying binary operators
  Vec3& operator+= (const Vec3 a);
  Vec3& operator-= (const Vec3 a);
  Vec3& operator*= (const float a);
  Vec3& operator/= (const float a);
};

// Define more operators in terms of basic ones
Vec3 operator+ (const Vec3 a, const Vec3 b);
Vec3 operator- (const Vec3 a, const Vec3 b);
Vec3 operator* (const Vec3 a, const float b);
Vec3 operator/ (const Vec3 a, const float b);

// Scalar * vector is commutative
inline Vec3 operator* (const float a, const Vec3 b) { return b * a; } 

// Vector multiplication is a special case
float operator*  (const Vec3 a, const Vec3 b);  // Scalar product
Vec3  operator%  (const Vec3 a, const Vec3 b);  // Vector product

// Output
ostream& operator<< (ostream &s, const Vec3 a);

/*****************************************************************************/

#endif









