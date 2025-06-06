/*
  Utility class to write image data to files
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

#ifndef _IMAGE_WRITER_H
#define _IMAGE_WRITER_H

#include <string>

#include "binsim_stdinc.h"

using std::string;

/*****************************************************************************/

/*
  Abstract superclass to define output functionality
*/
class Image_writer {
protected:
  // Image dimensions
  int width, height;

  // Storage for one line as one byte per colour per pixel
  unsigned char *jpeg_scanline;

#ifndef NOJPEG
  // Populate scanline with given row
  virtual void get_jpeg_scanline(const int row) = 0;
#endif

  // Get colour components as bytes
  virtual unsigned char get_red(const int column, const int row) = 0;
  virtual unsigned char get_green(const int column, const int row) = 0;
  virtual unsigned char get_blue(const int column, const int row) = 0;
public:
  // Constructor
  Image_writer(const int width1, const int height1)
    : width(width1), height(height1) 
  { jpeg_scanline = new unsigned char[width*3]; }

#ifndef NOJPEG
  // Write Jpeg image
  void write_jpeg(const string filename, const int quality);
#endif

  // Write PPM image
  void write_ppm(const string filename);

  // Allowed formats
  enum { PPM, JPEG};
};

/*****************************************************************************/

/*
  Image writer for framebuffer images
*/
class FB_image_writer : public Image_writer {
#ifndef NOJPEG
  // Populate scanline with given row
  void get_jpeg_scanline(const int row); 
#endif

  // Get colour components as bytes
  unsigned char get_red(const int column, const int row);
  unsigned char get_green(const int column, const int row);
  unsigned char get_blue(const int column, const int row);
public:
  // Constructor
  FB_image_writer(const int width1, const int height1)
    : Image_writer(width1, height1) { }
};

/*****************************************************************************/

/*
  Image writer for off-screen images with 8 bits per colour component
*/
class OS_image_writer : public Image_writer {
  // Pointer to image buffer
  const GLubyte *buffer;

#ifndef NOJPEG
  // Populate scanline with given row
  void get_jpeg_scanline(const int row);
#endif

  // Get colour components as bytes
  unsigned char get_red(const int column, const int row);
  unsigned char get_green(const int column, const int row);
  unsigned char get_blue(const int column, const int row);
public:
  // Constructor
  OS_image_writer(const int width1, const int height1, const GLubyte *buffer1)
    : Image_writer(width1, height1), buffer(buffer1) { }
};

/*****************************************************************************/

/*
  Image writer for off-screen images with 16 bits per colour component
*/
class OS16_image_writer : public Image_writer {
  // Pointer to image buffer
  const GLushort *buffer;

#ifndef NOJPEG
  // Populate scanline with given row
  void get_jpeg_scanline(const int row); 
#endif

  // Get colour components as bytes
  unsigned char get_red(const int column, const int row);
  unsigned char get_green(const int column, const int row);
  unsigned char get_blue(const int column, const int row);
public:
  // Constructor
  OS16_image_writer(const int width1, const int height1, 
		    const GLushort *buffer1)
    : Image_writer(width1, height1), buffer(buffer1) { }
};

/*****************************************************************************/

#endif
