/*
  Class to hold and render a grid defining the surface of a Roche lobe
  filling star
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

#ifndef _LOBE3D_H
#define _LOBE3D_H

#include <vector>

#include "bbcolormodel.h"
#include "object3d.h"

#include "binsim_stdinc.h"

/*****************************************************************************/

class Lobe_3d : public Object_3d {
  // Define the surface grid
  int n_steps, n_lat, n_long;

public:
  // Constructor
  Lobe_3d(const int n_steps1, const vector<float> phase, const float q, 
	  const float inclination, const float period, 
	  const float mass, const float t_pole, 
	  const float t_min,
	  const float l_irrad, const float disc_thick,
	  const float granulation_amplitude, 
	  const float granulation_period,
	  const float fill, BB_color_model &cm, const bool primary);
};

/*****************************************************************************/

#endif
