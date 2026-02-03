# Simuproc Development Environment

![Language](https://img.shields.io/badge/language-C%2B%2B17-blue.svg)
![Target](https://img.shields.io/badge/target-Simuproc-orange.svg)
![Status](https://img.shields.io/badge/status-In%20Development-yellow.svg)

A monorepo containing tools for Simuproc 1.4.2.0 development: a C compiler and Simuproc emulator.

## 📦 Projects

This repository contains two independent projects:

### [SCC - Simuproc C Compiler](scc/README.md)

A custom C compiler that targets the Simuproc 1.4.2.0 architecture. Compiles a subset of C into Simuproc assembly.

[→ Read more](scc/README.md)

### [Simuproc Emulator](sp-cli/README.md)

A CLI CPU emulator for the Simuproc 1.4.2.0 architecture. Executes Simuproc assembly code headless, enabling test automation for the compiler.

**Key Features:**

- Full Simuproc instruction set support
- Memory simulation
- IO through standard input and output

[→ Read more](sp-cli/README.md)

## 🛠️ Building the Projects

There are a total of 6 CMake presets defined, 3 for linux and 3 for windows. the presets are:

- linux-debug
- linux-test
- linux-release
- windows-debug
- windows-test
- windows-release

To build the project, do the following

1. Set a CMake preset. this will create the cmake files in the build/{preset} directory

```bash
cmake --preset {preset}
```

2. Build project

```bash
cmake --build build/{preset}
```


## 🧪 Testing

Ensure that you have built the project using the `{platform}-test` preset.

1. Run all tests

```bash
ctest --preset {platform}-test
```

2. Run scc tests or sp-cli tests independently

```bash
ctest --preset {platform}-test -L scc       # scc tests
ctest --preset {platform}-test -L sp-cli    # sp-cli tests
```

## 📖 Documentation

- [Compiler Documentation](scc/README.md)
- [Emulator Documentation](sp-cli/README.md)
- [Development Roadmap](ROADMAP.md)
- [Simuproc Architecture Reference](https://sites.google.com/site/simuproc/)

## 🗂️ Repository Structure

```
.
├── scc/                  # Simuproc C Compiler
│   ├── lexer/
│   └── main.cpp
├── sp-cli/               # Simuproc Emulator
│   ├── .../
│   └── main.cpp
├── tests/                # Integration tests
└── CMakeLists.txt        # Root build configuration
```

## 📚 References

* [Simuproc](https://sites.google.com/site/simuproc/)
* [Writing a C Compiler - Nora Sandler](https://norasandler.com/2017/11/29/Write-a-Compiler.html)
* [learncpp](https://www.learncpp.com/)