/*
  Class to hold and render a grid defining an accretion stream

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

#ifndef _STREAM3D_H
#define _STREAM3D_H

#include <vector>

#include "mathvec.h"
#include "stream.h"
#include "transparent_object3d.h"

#include "binsim_stdinc.h"

/*****************************************************************************/

class Stream_3d : public Transparent_object_3d {
  // Calculated accretion stream
  vector<Vec3> stream;

  // Define the surface grid
  int n_phi;
public: 
  // Constructor
  Stream_3d(const int n_phi1, vector<float> phase, const float q, 
	    const float inclination, const float m_prim, 
	    const float period, const float disc_rad, 
	    const float t_pole, const float max_stream_thick,
	    const float open_angle,
	    const float red, const float green,
	    const float blue, const float opacity);
};

/*****************************************************************************/

#endif
