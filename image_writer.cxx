/*
  Utility class to write image data to files

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

#include <cstdio>

#include <iostream>

#include <GL/glut.h>

#ifndef NOJPEG
extern "C" {
#include <jpeglib.h>
}
#endif

#include "image_writer.h"

using std::cout;

/*****************************************************************************/

#ifndef NOJPEG
void Image_writer::write_jpeg(const string filename, const int quality)
{
  // Pointer to row storage 
  JSAMPROW scanline_ptr[1];
  scanline_ptr[0] = jpeg_scanline;

  // Allocate and initialise JPEG compression object
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);

  // Specify destination for compressed data
  FILE *outfile;
  if ((outfile = fopen(filename.c_str(), "wb")) == NULL) 
    exit(1);
  jpeg_stdio_dest(&cinfo, outfile);

  // Set compression parameters
  cinfo.image_width = width;
  cinfo.image_height = height;
  cinfo.input_components = 3;
  cinfo.in_color_space = JCS_RGB;
  jpeg_set_defaults(&cinfo);
  jpeg_set_quality (&cinfo, quality, TRUE);

  // Start compression
  jpeg_start_compress(&cinfo, TRUE);

  // Write scanlines
  for (int i = 0 ; i < height ; i++) {
    get_jpeg_scanline(i);
    jpeg_write_scanlines(&cinfo, scanline_ptr, 1);
  }
  
  // Finish compression
  jpeg_finish_compress(&cinfo);
  jpeg_destroy_compress(&cinfo);

  // Close output file
  fclose(outfile);
}
#endif

/*****************************************************************************/

/*
  PPM writer adapted from osdemo.c in Mesa distribution (Joerg
  Schmalzl, Brian Paul)
*/
void Image_writer::write_ppm(const string filename)
{
  // Specify destination for image data
   FILE *outfile; 
   if ((outfile = fopen(filename.c_str(), "w")) == NULL) 
     exit(1);

   // Write header
   fprintf(outfile, "P6\n");
   fprintf(outfile, "# PPM file created by BinSim\n");
   fprintf(outfile, "%i %i\n", width, height);
   fprintf(outfile, "255\n");

   // Reopen in binary append mode
   fclose(outfile);
   if ((outfile = fopen(filename.c_str(), "ab")) == NULL) 
     exit(1);

   // Write each component of each pixel in turn
   for (int y = 0 ; y < height ; y++) {
     for (int x = 0 ; x < width ; x++) {
       // Need RGB on Unix, GBR on Windows.  Why?!
#ifdef WIN32
       fputc(get_green(x,y), outfile);
       fputc(get_blue(x,y), outfile);
       fputc(get_red(x,y), outfile);
#else
       fputc(get_red(x,y), outfile);
       fputc(get_green(x,y), outfile);
       fputc(get_blue(x,y), outfile);

#endif
     }
   }
   
   // Close output file
   fclose(outfile);
}

/*****************************************************************************/

#ifndef NOJPEG
/*
  Populate scanline with given row from framebuffer
*/
void FB_image_writer::get_jpeg_scanline(const int row) 
{
  // Read whole scanline in one go
  glReadPixels(0, height-row-1, width, 1, GL_RGB, GL_UNSIGNED_BYTE, 
	       jpeg_scanline); 
}
#endif

unsigned char FB_image_writer::get_red(const int column, const int row)
{
  // Read just red component
  unsigned char result;
  glReadPixels(column, height-row-1, 1, 1, GL_RED, GL_UNSIGNED_BYTE, 
	       &result);
  return result;
}

unsigned char FB_image_writer::get_green(const int column, const int row)
{
  // Read just green component
  unsigned char result;
  glReadPixels(column, height-row-1, 1, 1, GL_GREEN, GL_UNSIGNED_BYTE, 
	       &result);
  return result;
}

unsigned char FB_image_writer::get_blue(const int column, const int row)
{
  // Read just blue component
  unsigned char result;
  glReadPixels(column, height-row-1, 1, 1, GL_BLUE, GL_UNSIGNED_BYTE, 
	       &result);
  return result;
}

/*****************************************************************************/

#ifndef NOJPEG
/*
  Populate scanline with given row from 32bpp off-screen buffer
*/
void OS_image_writer::get_jpeg_scanline(const int row) 
{
  for (int j = 0 ; j < width ; j++) {
    // Base index for pixel
    int buffer_index = ((height-row-1)*width + j) * 4;

    // One colour component at a time
    jpeg_scanline[j*3]   = buffer[buffer_index];
    jpeg_scanline[j*3+1] = buffer[buffer_index+1];
    jpeg_scanline[j*3+2] = buffer[buffer_index+2];
  }
}
#endif

unsigned char OS_image_writer::get_red(const int column, const int row)
{
  // Read just red component
  return buffer[((height-row-1)*width + column) * 4];
}

unsigned char OS_image_writer::get_green(const int column, const int row)
{
  // Read just green component
  return buffer[((height-row-1)*width + column) * 4 + 1];
}

unsigned char OS_image_writer::get_blue(const int column, const int row)
{
  // Read just blue component
  return buffer[((height-row-1)*width + column) * 4 + 2];
}

/*****************************************************************************/

#ifndef NOJPEG
/*
  Populate scanline with given row from  64bpp off-screen buffer
*/
void OS16_image_writer::get_jpeg_scanline(const int row) 
{
  for (int j = 0 ; j < width ; j++) {
    // Base index for pixel
    int buffer_index = ((height-row-1)*width + j) * 4;

    // One colour component at a time
    jpeg_scanline[j*3]   = (buffer[buffer_index] >> 8);
    jpeg_scanline[j*3+1] = (buffer[buffer_index+1] >> 8);
    jpeg_scanline[j*3+2] = (buffer[buffer_index+2] >> 8);
  }
}
#endif

unsigned char OS16_image_writer::get_red(const int column, const int row)
{
  // Read just red component
  return buffer[((height-row-1)*width + column) * 4] >> 8;
}

unsigned char OS16_image_writer::get_green(const int column, const int row)
{
  // Read just green component
  return buffer[((height-row-1)*width + column) * 4 + 1] >> 8;
}

unsigned char OS16_image_writer::get_blue(const int column, const int row)
{
  // Read just blue component
  return buffer[((height-row-1)*width + column) * 4 + 2] >> 8;
}


