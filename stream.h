/*
  Class to calculate trajectory of an accretion stream
  Released as part of BinSim 0.9
  This release built on 18 January 2005

  Author: Robert I. Hynes (rih@astro.as.utexas.edu)
          The University of Texas at Austin
          Department of Astronomy
          Austin
          Texas, USA

  Copyright (C) 2005 Robert I. Hynes

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef _STREAM_H
#define _STREAM_H

#include <vector>

#include "mathvec.h"

#include "binsim_stdinc.h"

using std::vector;

/*****************************************************************************/

class Stream {
private:
  // Binary parameters
  float q, m_prim, m_sec, gm1, gm2;
  float period, omega;
  float a, a1, a2, al1, r_egg;

  // Timestep
  float dt;

  // Normalised midpoint stream speed = delta_x/a / delta_t/P
  float speed;
public:
  // Constructor
  Stream(const float q1, const float m1, const float period1);

  // Calculate a stream trajectory
  vector<Vec3> stream_calc(const float dl, const float r_max);

  // Return the stream speed
  float get_speed() { return speed; }
};

/*****************************************************************************/

#endif
