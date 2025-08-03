# Hydrogen

Hydrogen is a simple, programming language implemented in C++. This project is a personal exploration of how compilers work, following along with [Matthew Oros's YouTube series](https://www.youtube.com/playlist?list=PLUDlas_Zy_qC7c5tCgTMYq2idyyT241qs) and the corresponding [GitHub repository](https://github.com/orosmatthew/hydrogen-cpp).

The language supports exit statement and variable declarations. Future goals include implementing arithmetic operations, if-else scopes, control flow (while/for loops) and user-defined functions.

---

## Getting Started

### Prerequisites

To build and run this project, you need:

* `cmake`
* `nasm`
* `ld`

> Note: If you're using **Windows**, you must run this project inside **WSL**.
> On **Linux**, it should work out of the box as long as the required tools are installed.

### Installation

1. Clone this repository:

   ```bash
   git clone <your fork or this repo>
   cd hydrogen
   ```

2. Create a build directory:

   ```bash
   mkdir build
   cd build
   ```

3. Make sure the following tools are installed:

   ```bash
   cmake --version
   nasm -v
   ld -v
   ```

   Install them if they're missing:

   ```bash
   sudo apt update
   sudo apt install cmake nasm binutils
   ```

4. Build and run the project:

   ```bash
   cmake -S . -B build && cmake --build build && ./build/hydro test.hy
   ```

This will compile the `.hy` file and output the result (exit code).

---

## Sample Code

```hy
let y = 4;
let x = 7;

exit(x)
```

---

## Credits

Thanks to [Matthew Oros](https://github.com/orosmatthew) for the excellent compiler tutorial series.

---

## License

This project follows the original license from the tutorial unless otherwise specified.
