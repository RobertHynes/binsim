/*
  Header file containing assorted constants and units

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

#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#include "binsim_stdinc.h"

/*****************************************************************************/

namespace Sci_const {
  // Pi
  const float PI =     3.1415927f;

  // Physical constants
  const float K =      1.380658e-23f;           // Boltzmann's constant
  const float G =      6.67259e-11f;            // Gravitational constant
  const float H =      6.6260755e-34f;          // Planck's constant
  const float HBAR =   1.0545727e-34f;          // Planck's constant / 2 pi
  const float C =      2.9979246e8f;            // Speed of light
  const float S =      5.67051e-8f;             // Stefan's Constant
  const float E0 =     8.8541878e-12f;          // Permittivity of free space
  const float MU0 =    1.2566371e-6f;           // Permeability of free space
  const float E =      1.6021773e-19f;          // Electron charge
  const float U =      1.6605402e-27f;          // Atomic mass unit
  const float ME =     9.1093897e-31f;          // Electron mass
  const float MP =     1.672623e-27f;           // Proton mass
  const float MN =     1.6749286e-27f;          // Neutron mass
  const float ST =     6.653e-29f;              // Thomson cross section

  // Distance units
  const float ANG =    1.0e-10f;                // Angstrom
  const float NM =     1.0e-9f;                 // Nanometre
  const float MICRON = 1.0e-6f;                 // Micrometre
  const float MM =     1.0e-3f;                 // Millimetre
  const float CM =     1.0e-2f;                 // Centimetre
  const float M =      1.0f;                    // Metre
  const float KM =     1.0e3f;                  // Kilometre
  const float RSUN =   6.9599e8f;               // Solar radius
  const float AU =     1.495979e11f;            // Astronomical unit
  const float PC =     3.085678e16f;            // Parsec
  const float KPC =    3.085678e19f;            // Kiloparsec
  const float MPC =    3.085678e22f;            // Megaparsec
  const float LY =     9.46053e15f;             // Lightyear

  // Time units
  const float SECOND = 1.0f;                    // Second
  const float MIN =    6.0e1f;                  // Minute
  const float HOUR =   3.6e3f;                  // Hour
  const float DAY =    8.64e4f;                 // Day
  const float YEAR =   3.1557e7f;               // Year

  // Mass units
  const float GRAM =   1.0e-3f;                 // Gram
  const float KG =     1.0f;                    // Kilogram
  const float MSUN =   1.9891e30f;              // Solar mass

  // Energy units
  const float J =      1.0f;                    // Joule
  const float ERG =    1.0e-7f;                 // Erg

  // Power units
  const float LSUN =   3.826e26f;               // Solar luminosity

  // Angular units
  const float DEG =    1.74533e-2f;             // Degree
  const float RAD =    1.0f;                    // Radian

  // Double precision versions
  const double dPI =     3.1415926535897932;
  const double dK =      1.380658e-23;           
  const double dG =      6.67259e-11;            
  const double dH =      6.6260755e-34;          
  const double dHBAR =   1.05457266e-34;         
  const double dC =      2.99792458e8;           
  const double dS =      5.67051e-8;             
  const double dE0 =     8.854187817e-12;        
  const double dMU0 =    1.25663706e-6;          
  const double dE =      1.60217733e-19;         
  const double dU =      1.6605402e-27;          
  const double dME =     9.1093897e-31;          
  const double dMP =     1.672623e-27;           
  const double dMN =     1.6749286e-27;          
  const double dST =     6.653e-29;              
  const double dANG =    1.0e-10;                
  const double dNM =     1.0e-9;                 
  const double dMICRON = 1.0e-6;                 
  const double dMM =     1.0e-3;                 
  const double dCM =     1.0e-2;                 
  const double dM =      1.0;                    
  const double dKM =     1.0e3;                  
  const double dRSUN =   6.9599e8;               
  const double dAU =     1.495979e11;            
  const double dPC =     3.085678e16;            
  const double dKPC =    3.085678e19;            
  const double dMPC =    3.085678e22;            
  const double dLY =     9.46053e15;             
  const double dSECOND = 1.0;                    
  const double dMIN =    6.0e1;                  
  const double dHOUR =   3.6e3;                  
  const double dDAY =    8.64e4;                 
  const double dYEAR =   3.1557e7;               
  const double dGRAM =   1.0e-3;                 
  const double dKG =     1.0;                    
  const double dMSUN =   1.9891e30;              
  const double dJ =      1.0;                    
  const double dERG =    1.0e-7;                 
  const double dLSUN =   3.826e26;               
  const double dDEG =    1.74533e-2;             
  const double dRAD =    1.0;                     
}

/*****************************************************************************/

#endif
