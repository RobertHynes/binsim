/*
  Class to calculate trajectory of an accretion stream

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

#include "constants.h"
#include "roche.h"
#include "stream.h"

/*****************************************************************************/

/*
  Constructor to initialise the stream model
*/
Stream::Stream(const float q1, const float m1, const float period1)
{
  using namespace Sci_const;

  // Assign binary parameters
  q = q1;

  m_prim = m1;
  m_sec = m1 * q;

  gm1    = G * m_prim;
  gm2    = G * m_sec;
  period = period1;
  omega  = 2.0 * PI / period;

  // Calculate sensible timestep
  dt = 2e-6 * period;

  // Create Roche lobe object and derive more system parameters
  Roche_lobe lobe(1.0f/q, period, m_prim);

  a     = lobe.get_separation();
  a1    = a * lobe.get_c_of_m();
  a2    = a - a1;
  al1   = a * lobe.get_l1();

  r_egg = lobe.get_eggleton() * a;
}

/*
  Calculate a stream trajectory
*/
vector<Vec3> Stream::stream_calc(const float dl, const float r_max)
{
  // Integated time
  float t = 0.0f;

  // Stream positions and times
  vector<Vec3> result;
  vector<float> times;

  // Position and velocity vectors
  Vec3 pos;
  pos.y = al1 - a1 - 0.001 * a ;

  Vec3 vel;
  vel.x = -omega * pos.y;

  Vec3 dr1, dr2, drl1, dv;

  // Points in corotating frame
  Vec3 first_point, last_point, new_point;

  // Store initial coordinates
  first_point.x = -pos.y / a;
  first_point.y = pos.x / a;
  last_point = first_point;
  result.push_back(first_point);  

  long i = 0;

  const float maxit = 2e5;

  float moddr1, moddr2, moddrl1;

  // Iterate over steps
  do {
    // Get sine and cosine of phase angle
    float angle = omega * i * dt;
    float sine   = sin(angle);
    float cosine = cos(angle);
        
    // Get displacements from stellar components
    dr1 = pos + a1 * Vec3(-sine, cosine, 0.0f);
    dr2 = pos + a2 * Vec3(sine, -cosine, 0.0f);
    drl1 = pos + al1 * Vec3(sine, -cosine, 0.0f);
      
    moddr1  = dr1.mod();
    moddr2  = dr2.mod();
    moddrl1 = drl1.mod();
    
    // Calculate velocity change due to gravity
    dv = -dt * (gm1 * dr1 / moddr1 / moddr1 / moddr1 +
		gm2 * dr2 / moddr2 / moddr2 / moddr2);
        
    // Update position and velocity
    pos += vel * dt;
    vel += dv;

    // Update time
    t += dt;

    // Rotate positions and velocities so they are relative to line of
    // centres, convert to units of binary separation or km/s and store
    // in return array
    new_point.x = (pos.x  * sine   - pos.y  * cosine) / a;
    new_point.y = (pos.x  * cosine + pos.y  * sine) / a;
    
    // Check if point is to be stored
    if ((new_point - last_point).mod() >= dl) {
      result.push_back(new_point);
      last_point = new_point;
      times.push_back(t);
    }
    
    i++;
  } while(moddr1 > r_max * r_egg && i < maxit);

  // Calculated normalised midpoint stream speed
  int mid_index = result.size() / 2;
  float delta_pos = (result[mid_index+1] - result[mid_index]).mod();
  float delta_t = (times[mid_index+1] - times[mid_index]) / period;
  speed = delta_pos / delta_t;

  // Return trajectory
  return result;
}

