# PDBlend, a 2D Peridynamic Solver

This repository contains a Peridynamic solver for simulating failure and fracture processes in two-dimensional solid domains. The code implements a blended-Peridynamics approach that efficiently captures crack initiation and propagation mechanisms. As a demonstration, the provided input files are configured to simulate a three-point bending test of a concrete beam under monotonic loading conditions.


## Prerequisites

- C++11 compatible compiler (g++ is preferred)
- Linux/macOS environment

## Installation

Clone this repository to your local machine:

```bash
git clone https://github.com/SemsiCoskun/PDBlend.git
cd PDBlend
```

## How to Run

1. Make the script executable:
   ```bash
   g++ -std=c++11 -Wall *.cpp -o PDBlend_Solver -fopenmp
   ```
   - ```g++```:The C++ compiler.
   - ```-std=c++11```: Ensures compilation with C++11 standard features.
   - ```-Wall```: Enables all warning messages (recommended for robust code).
   - ```*.cpp```: Compiles all ```.cpp``` source files in the current directory.
   - ```-o PDBlend_Solver```: Specifies the output executable name as ```PDBlend_Solver```.
   - ```-fopenmp```: Crucial for enabling OpenMP parallelization in the solver.

3. Run the script:
   ```bash
   ./PDBlend_Solver
   ```

   The simulation will execute, process the inputs, and generate results.

## Understanding the Code

The simulation workflow in PDBlend conceptually involves setting up the problem (mesh, boundaries, loading) and then executing the peridynamic time-stepping solver. All necessary functionalities for both are co-located in the single codebase.

The solver's core functionality is encapsulated in several key C++ header files, each responsible for a specific part of the peridynamic simulation. This includes functions for problem setup and simulation execution:

- ```geometricalFeatures.h```: Provides utility functions and definitions related to geometrical calculations, used by other modules like ```createMesh.h``` and ```createBoundary.h```.
- ```ZonesBoundary.h```: Defines structures or classes for specifying boundary zones.
- ```ZonesCrackLines.h```: Defines structures or classes for specifying pre-existing crack lines.
- ```ZonesExclusion.h```: Defines structures or classes for specifying regions to be excluded from the mesh or neighbor searches.
- ```createMesh.h```: Generates the discretization (mesh) of the simulation domain, defining nodal coordinates based on ```geometricalFeatures.h``` and potentially considering ```ZonesExclusion.h``` for excluded regions.
-  ```createNeighbors.h```: Establishes the neighbor list for each node within its peridynamic horizon, crucial for bond definition and interaction. This includes handling ```ZonesCrackLines.h``` and ```ZonesExclusion.h```.
- ```createBoundary.h```: Defines and applies boundary conditions (fixed or assigned displacements, velocities, and forces) to the simulation domain, utilizing ```ZonesBoundary.h``` and ```geometricalFeatures.h``` for defining boundary regions.
- ```createLoading.h```: Manages the application of external loads over the simulation's loading steps.
- ```calculateDamage.h```: Manages the calculation of local damage accumulation and identifies newly broken bonds within the material.
- ```calculateDisplacement.h```: Computes nodal displacements, velocities, and accelerations based on internal and external forces, incorporating bond health degradation. The half-step time integration algorithm is utilized.
- ```calculateDt.h```: Determines the stable time step (```dt```) for the dynamic simulation based on material properties and discretization.
- ```calculateEffectiveHorizonArea.h```: Calculates the effective area of interaction for each node within its peridynamic horizon.
- ```calculatePartialAreas.h```: Computes partial areas, likely used in peridynamic force calculations or influence functions.
- ```degradate.h```: Implements the bond degradation law, determining the health of bonds based on critical bond strains (linear and bilinear models).

  
**Simulation Workflow**
  The simulation proceeds by first setting up the geometric domain, mesh, and boundary conditions, then iteratively calculating the dynamic response:

1. **Mesh Generation:** The domain is discretized into a set of nodes and their initial coordinates are defined (```createMesh```). This can include defining zones for exclusion.
2. **Neighbor Determination:** For each node, its neighbors within a defined horizon are identified, forming the "bonds" of the peridynamic model (```createNeighbors```). Pre-existing crack lines can influence bond creation.
3. **Boundary and Loading Application:** Boundary conditions (fixed nodes, applied displacements/velocities/forces) are established (```createBoundary```), and the external loading schedule is set up (```createLoading```).
4. **Iterative Solver:** The core simulation loop then begins, where at each time step:
   - Internal forces are calculated based on bond deformations and material properties.
   - Damage is assessed and bonds can degrade or break (```calculateDamage```, ```degradate```).
   - Nodal accelerations, velocities, and displacements are updated (```calculateDisplacement```).
5. **Output:** Relevant simulation data (e.g., displacements, damage) is periodically written to output files. 

## Contributing

If you'd like to contribute to this project:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/your-feature`)
3. Commit your changes (`git commit -m 'Add some feature'`)
4. Push to the branch (`git push origin feature/your-feature`)
5. Open a Pull Request

## License

This project is licensed under the GNU General Public License v3.0 (GPL-3.0) - see the [LICENSE](LICENSE) file for details.

Copyright (C) 2025

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://www.gnu.org/licenses/>.

## Contact

For questions, feedback, or collaboration inquiries, please contact:
- Email: semsicoskun@gmail.com
- GitHub: [SemsiCoskun](https://github.com/SemsiCoskun

## References

- Semsi Coskun and Davood Damircheli and Robert Lipton. "PDBlend: A Blended Peridynamics Solver for Fracture Simulation." arXiv preprint arXiv:2503.20109, 2025. https://arxiv.org/abs/2503.20109
- Silling, S., 2000. Reformulation of elasticity theory for discontinuities and long-range forces. Journal of the Mechanics and Physics of Solids 48, 175–209. doi:https://doi.org/10.1016/S0022-5096(99)00029-0.
- Underwood, P., 1983. Dynamic relaxation, in: Computational Methods in Mechanics, Vol 1: Computational methods for transient analysis. Elsevier Science Publishers B.V., pp. 245–265.
- D. Littlewood. Roadmap for software implementation. In Handbook of peridynamic modeling, pages 147–178. Chapman and Hall/CRC, 2016.
- Silling, S.A., Askari, E., 2005. A meshfree method based on the peridynamic model of solid mechanics. Computers & structures 83, 1526–1535. doi:https://doi.org/10.1016/j.compstruc.2004.11.026.
