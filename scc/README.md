# SCC: Simuproc C Compiler

![Language](https://img.shields.io/badge/language-C%2B%2B17-blue.svg)
![Target](https://img.shields.io/badge/target-Simuproc-orange.svg)
![Status](https://img.shields.io/badge/status-In%20Development-yellow.svg)

**SCC** is a custom compiler built from scratch in C++17. It compiles a specific subset of the C language into assembly code for **Simuproc**.

## 🎯 Project Scope

**SCC** aims to implement a robust subset of the C language. This project intends to support essential systems programming features targeting the **Simuproc** architecture.

### Core Features

* **Data Types:** Primary support for `int` and `char`.
* **Memory Management:** Full support for Pointers (`*`), Address-of (`&`), and manual memory manipulation.
* **Functions:** Support for defining functions, calling conventions, and parameter passing.
* **Scope & State:** Implementation of block scopes (including variable shadowing) and local/global variable storage.
* **Control Flow:** `if`, `else`, `while`, and `for` loops.

## ⛔ Limitations

The compiler focuses on the *language* itself rather than the *translation environment*. The following features are **out of scope**:

* **The Preprocessor:** No support for `#include`, `#define`, `#ifdef`, or macros. The compiler expects pure C code.
* **The Linker:** No multi-file compilation. The entire program must be contained within a single source file (translation unit).
* **Standard Library:** No libc support (e.g., no `stdio.h`, `string.h`). Output will be handled via intrinsic functions mapping to Simuproc I/O instructions.
* **Binary Arguments:** The entry point is strictly `int main()`. No command-line arguments (`argc`, `argv`) will be passed to the binary.

## 🛠️ Building the Project

*

## Using SCC

*
