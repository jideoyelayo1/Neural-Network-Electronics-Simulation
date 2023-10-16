# Neural Network Electronics Simulation

This project implements a neural network in C++ to simulate various electronic circuits, including XOR, NAND, MUX, DEMUX, ADDER, and FLIP FLOP. The neural network is designed from scratch and provides a foundation for simulating different electronics circuits. This README provides an overview of the project and explains how to use it.

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Prerequisites](#prerequisites)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [Configuration](#configuration)
- [Saving Results](#saving-results)
- [Contributing](#contributing)
- [License](#license)
- [Contact](#contact)

## Introduction

This project is a neural network implemented in C++ from scratch. It simulates various electronic circuits, including XOR, NAND, MUX, DEMUX, ADDER, and FLIP FLOP, using artificial neural networks. The purpose of this project is to gain a deeper understanding of neural networks and their application in simulating electronic circuits.

## Features

- Simulates electronic circuits using a custom neural network implementation.
- Supports various electronic circuit simulations, including XOR, NAND, MUX, DEMUX, ADDER, and FLIP FLOP.
- Provides the flexibility to define and test different topologies for neural networks.
- Saves simulation results to text files for analysis and evaluation.

## Prerequisites

To run this project, you need:

- A C++ compiler (e.g., g++)
- CMake (for building)
- A basic understanding of neural networks and electronic circuits

## Getting Started

1. Clone this repository to your local machine.
2. Build the project using CMake.
3. Run the compiled executable to simulate electronic circuits using the neural network.

## Usage

### Creating Test Files

You can create test files for various electronic circuits. Example Python scripts are provided for creating test files:

- XOR Test File: `XOR.txt`
- Flip-Flop Test File: `FlipFlop.txt`

Modify these scripts to generate test files for other electronic circuits if needed.

### Running the Neural Network

1. Compile and run the C++ program, providing the test file as input. For example:

   ```shell
   ./NeuralNet test/XOR.txt
   ```

2. The program will read the test file, simulate the electronic circuit using the neural network, and save the results in a text file.

3. The simulation results will include input values, output values, and error metrics.

## Configuration

You can configure the neural network topology, learning rate (eta), momentum (alpha), and other parameters in the C++ source code. The provided code is well-documented to guide you through the configuration process.

## Saving Results

The program saves simulation results in text files in the "results" folder. You can change the filename and location in the C++ code to match your requirements.

## Contributing

If you wish to contribute to this project, feel free to fork the repository, make your changes, and submit a pull request.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contact

For any inquiries or feedback, please contact [Your Name] at [Your Email Address].

---

Feel free to customize this README file with your name, contact information, and additional details as needed. It will serve as a useful reference for anyone interested in your project.