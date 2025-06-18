#ifndef GLOBALCONSTANTS_H
#define GLOBALCONSTANTS_H

#include <cmath>

// Control the number of threads to be used during parallelization
const int nThreads = 8;

// Material Constants
const double E = 33.89e+9; // Elastic modulus (Pa)
const double rho = 2500.0; // Density (kg/m^3)
const double nu = 1.0 / 3.0; // Poisson ratio
const double Gf = 116.15; // Fracture Energy (J/ m^2)
const double ft = 3.39e+6; // Ultimate Tensile Strength (Pa)
const int materialModel = 2;
const double dtSF = 0.85;

// Domain
const double crackLength = 45.0e-3;
const double span = 600.0e-3;
const double thickness = 50.0e-3;

const double gridSize = 2.0e-3;  // (m)
const double horizon = 6.0e-3;//gridSize * 3.0;

const double area = gridSize * thickness; // Cross-sectional area of the material point
const double vol = area * gridSize; // Volume of a material point
const double vol_0 = M_PI * horizon * horizon * thickness; // full neighborhood volume
const double boundaryLayer = gridSize*4.0;
const double domainX = 708.0e-3; // (m)
const double domainY = 150.0e-3; // (m)
const double characteristicLength = domainY;

// PD Material Constants
const double bc = 6.0 * E /((1.0-nu) * M_PI * std::pow(horizon,3) * thickness);
const double scLinear = std::sqrt(4.0 * M_PI * Gf / (9.0 * E * horizon));

// 
const double criticalBondHealth = 0.0001;
const double localDamping = 2e+07;
const double tolerance = 1e-05;
const double maxIterationNumber = 10e+3;
const int totalNumLoadStep = 200;
const double finalDisplacement = 0.80e-3;
const double numCycles = 0;
//

#endif