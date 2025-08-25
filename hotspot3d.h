/*
  Class to hold and render a grid defining a hot spot

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

#ifndef _HOTSPOT3D_H
#define _HOTSPOT3D_H

#include <vector>

#include "transparent_object3d.h"

#include "binsim_stdinc.h"

/*****************************************************************************/

class Hot_spot_3d : public Transparent_object_3d {
  // Define the surface grid
  int n_steps, n_theta, n_phi;
public:
  // Constructor
  Hot_spot_3d(const int n_steps1,
	      vector<float> phase, const float q, 
	      const float inclination, const float mass, 
	      const float period, const float r, const float size, 
	      const float red, const float green, 
	      const float blue, const float opacity,
	      const float timescale);
};

/*****************************************************************************/

#endif
