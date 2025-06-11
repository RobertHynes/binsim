/*
  Class to hold information about a binary visualisation
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

#ifndef _BINSIM_H
#define _BINSIM_H

#include <string>
#include <vector>

#include "binary3d.h"
#include "image_writer.h"
#include "keyword.h"
#include "movie_maker.h"
#include "starsky.h"

#include "binsim_stdinc.h"

/*****************************************************************************/

class Bin_sim {
  // Animation frame counter
  int phase_index;

  // Number of phases
  int n_phase;

  // Starfield object
  Star_sky *sky;

  // Image writer
  Image_writer *writer;

  // MPEG movie maker
  Movie_maker *animator;

  // Binary object
  Binary_3d *binary;

  // Draw objects
  void gl_commands(void);
public:
  // Image quality options
  bool high_quality, hq_antialias, antialias;
  int width, height, n_samples;

  // View options
  float scale, aspect_ratio;
  float world_width, world_height, world_pixsize;
  float world_centre_x, world_centre_y;
  float world_min_x, world_max_x, world_min_y, world_max_y;
  vector <float> phase;

  // Animation options
  bool anim;

  // Output options
  bool save;
  int jpeg_quality;
  string imagefile, anim_root;
  bool vertex_log;
  int output_format;

  // Starfield options
  bool show_stars;
  int n_star;
  float star_colour_range, star_size;

  // Constructor
  Bin_sim(Key_list &params, Image_writer *writer1);

  // Draw image
  void draw(const bool onscreen);

  // Initialisation of OpenGL
  void gl_setup(const bool onscreen);

  // Check for last frame
  bool last_frame() { return (phase_index == n_phase - 1); }

  // Go to the next frame
  void next_frame() { if (phase_index < n_phase-1) phase_index++; }
};

/*****************************************************************************/

#endif
