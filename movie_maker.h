/*
  Class to assemble a group of images into an MPEG movie.
  
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

#ifndef _MOVIE_MAKER_H
#define _MOVIE_MAKER_H

#include <fstream>
#include <string>

#include "keyword.h"

#include "binsim_stdinc.h"

using std::ofstream;
using std::string;

/*****************************************************************************/

/*
  Class to assemble a group of images into an MPEG movie
*/
class Movie_maker {
  // Filename of MPEG to create
  string mpeg_filename;

  // MPEG Pattern
  string mpeg_pattern;

  // MPEG Q-scales
  string mpeg_iqscale, mpeg_pqscale, mpeg_bqscale;

  // MPEG parameter file
  ofstream *param_file;

  // Directory to hold temporary images
  string root_dir;

  // Write beginning and end of MPEG parameter file
  void write_mpeg_header();
  void write_mpeg_trailer();
public:
  // Constructor
  Movie_maker(Key_list &params);

  // Add an image to the parameter file
  void add_image_to_mpeg(const string filename);

  // Create the MPEG from the parameter file
  void make_mpeg();
};

/*****************************************************************************/

#endif
