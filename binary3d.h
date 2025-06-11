/*
  Class to describe 3D model of an interacting binary
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

#ifndef _BINARY3D_H
#define _BINARY3D_H

#include <vector>

#include "bbcolormodel.h"
#include "constants.h"
#include "corona3d.h"
#include "disc3d.h"
#include "hotspot3d.h"
#include "jet3d.h"
#include "keyword.h"
#include "lobe3d.h"
#include "stream3d.h"
#include "transparent_disc3d.h"

#include "binsim_stdinc.h"

/*****************************************************************************/

class Binary_3d {
public:
  // Flags for components to display
  bool show_lobe1, show_lobe2, show_disc, show_transparent_disc;
  bool show_stream, show_hot_spot;
  bool show_corona1, show_corona2, show_stellar_wind, show_jet;

  // Basic binary parameters
  float period, q, m_prim, inclination;

  // Primary star parameters
  int lobe1_n_steps;
  float lobe1_fill, lobe1_t_pole, lobe1_t_min;
  float lobe1_granulation, lobe1_granulation_period;

  // Companion star parameters
  int lobe2_n_steps;
  float lobe2_fill, lobe2_t_pole, lobe2_t_min;
  float lobe2_granulation, lobe2_granulation_period;

  // Irradiation parameters
  float luminosity1, luminosity2, disc_eff_thick;

  // Disc parameters
  int disc_n_steps;
  float disc_rad, disc_r_in, disc_geom_thick;
  float disc_tout, disc_temp_grad, disc_beta;
  int disc_n_flare, disc_flare_length;

  // Optically thin disc parameters
  int transparent_disc_n_steps; 
  float transparent_disc_rad, transparent_disc_r_in;
  float transparent_disc_geom_thick, transparent_disc_beta;
  float transparent_disc_red, transparent_disc_green;
  float transparent_disc_blue, transparent_disc_opacity;
  int transparent_disc_n_flare, transparent_disc_flare_length;
  float transparent_disc_hot_opacity;

  // Stream parameters
  float stream_max_thick, stream_open_angle;
  float stream_red, stream_green, stream_blue, stream_opacity;
  float stream_disc_rad;

  // Hot spot parameters
  float hot_spot_size, hot_spot_red, hot_spot_green, hot_spot_blue;
  float hot_spot_opacity, hot_spot_temp, hot_spot_timescale;
  float hot_spot_disc_rad;

  // Corona parameters
  float corona1_rad, corona1_red, corona1_green, corona1_blue;
  float corona1_opacity, corona1_exp;
  float corona2_rad, corona2_red, corona2_green, corona2_blue;
  float corona2_opacity, corona2_exp;

  // Stellar wind parameters
  float stellar_wind_rad;
  float stellar_wind_red, stellar_wind_green, stellar_wind_blue;
  float stellar_wind_opacity, stellar_wind_exp;

  // Jet parameters
  float jet_opening_angle, jet_opacity, jet_exp;
  float jet_red1, jet_green1, jet_blue1;
  float jet_red2, jet_green2, jet_blue2;
  float jet_inc, jet_phi;

  // Phases
  vector<float> phase;

  // Binary components
  Lobe_3d *lobe1;
  Lobe_3d *lobe2;
  Disc_3d *disc;
  Transparent_disc_3d *transparent_disc;
  Stream_3d *stream;
  Hot_spot_3d *hot_spot;
  Corona_3d *corona1;
  Corona_3d *corona2;
  Corona_3d *stellar_wind;
  Jet_3d *jet;

  // Constructor
  Binary_3d(Key_list &params, vector<float> phase1);

  // Draw binary components
  void draw(int phase_index);

  // Read in parameters from file
  void get_params(Key_list &params);
};

/*****************************************************************************/

#endif
