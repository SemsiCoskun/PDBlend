# PDBlend, a 2D Peridynamic Solver

This repository contains a Peridynamic solver for simulating failure and fracture processes in two-dimensional solid domains. The code implements a blended-Peridynamics approach that efficiently captures crack initiation and propagation mechanisms. As a demonstration, the provided input files are configured to simulate a three-point bending test of a concrete beam under monotonic loading conditions.

## Project Structure

The project is organized into the following directories:

- **Input Creator**: Creates the input files required for the simulation
- **Solver**: Contains the main simulation code that processes the inputs and produces results
- **IO**: Contains input and output folders for data exchange between the modules

## Prerequisites

- C++11 compatible compiler (g++ is preferred)
- Linux/macOS environment (for bash script)

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

2. Run the script:
   ```bash
   ./PDBlend_Solver
   ```

## Understanding the Code

The simulation workflow consists of two main steps:

1. **Input Creation**: Generates necessary input files including material properties, geometry, loading conditions, and global constants.

2. **Solver Execution**: Processes the inputs to simulate the three-point bending test and produces results.

## Output

The simulation results are stored in the `3-IO/output` directory (or your custom IO output directory).

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
