# Project Roadmap: C Compiler for Simuproc Architecture

This project aims to implement a C compiler following Nora Sandler's *"Writing a C Compiler"*, but targeting the **Simuproc** instead of x86-64.

## 🛠️ Phase 0: The Infrastructure

*Goal: Create a CLI utility to emulate Simuproc behavior. Since the official Simuproc is GUI-only, a scriptable emulator is required for automated compiler testing.*

- [x] **Architecture Implementation**
  - [x] Implement Memory Array: 4096 words (0x000 - 0xFFF).
  - [x] Implement General Purpose Registers (AX, BX, CX).
  - [x] Implement Addressing Registers (PC, SP, BP, MAR).
  - [x] Implement remaining registers (IR, control flags).
- [x] **Instruction Set Decoder**
  - [x] Implement Data Movement: `MOV`, `LDA`, `STA`, `LDB`, `STB`.
  - [x] Implement Arithmetic: `ADD`, `SUB`, `MUL`, `DIV` (and their Float variants).
  - [x] Implement Logic/Control: `AND`, `OR`, `NOT`, `CMP`, `JMP`, `JEQ`, `JNE`, etc.
- [x] **Instruction Cycle**
  - [x] Implement Instruction Cycle (Fetch Cycle)
- [ ] **Test Runner Interface**
  - [x] CLI Argument Parsing.
  - [x] txt. reading and parsing into memory.
  - [ ] Output State: Dump the final complete state of the emulator.

---

## 🏗️ Phase 1: The Basics

*Goal: Implement Part I of the book.*

### Chapter 1: A Minimal Compiler

- [ ] Lexer: Tokenize basic C syntax.
- [ ] Parser: Generate AST for a generic `main` function returning an int.
- [ ] Code Gen: Emit Simuproc assembly to put a constant in the Accumulator/Return register.
- [ ] Test: Verify `return 42;` works in the emulator.

### Chapter 2: Unary Operators

- [ ] Update Lexer/Parser for `-` (negation) and `~` (bitwise complement).
- [ ] Implement TACKY (Intermediate Representation) generation.
- [ ] Code Gen: Implement `NEG` and `NOT` logic using Simuproc instructions.

### Chapter 3: Binary Operators

- [ ] Support `+`, `-`, `*`, `/`.
- [ ] Implement precedence climbing in the Parser.
- [ ] Code Gen: Manage registers for arithmetic operations.

### Chapter 4: Logical and Relational Operators

- [ ] Support `&&`, `||`, `!`, `>`, `==`, `!=`.
- [ ] Code Gen: Implement comparisons and conditional jumps (`CMP`, `JNE`, `JEQ`, etc.).

### Chapter 5: Local Variables

- [ ] Stack Management.
  - [ ] Implement logic to calculate stack offsets for variables.
  - [ ] Code Gen: Use `push` and `pop` from Simuproc instruction set to manage stack.
- [ ] Semantic Analysis: Add pass to check for undeclared variables.

### Chapter 6: If Statements & Conditional Expressions

- [ ] Support `if`, `else`, and ternary `? :`.
- [ ] Code Gen: Label generation and control flow graph management.

### Chapter 7: Compound Statements

- [ ] Support block scopes `{ ... }`.
- [ ] Semantic Analysis: Handle variable shadowing and scope resolution.

### Chapter 8: Loops

- [ ] Support `while`, `do-while`, `for`.
- [ ] Support `break` and `continue`.
- [ ] Semantic Analysis: Ensure break/continue are only used inside loops.

### Chapter 9: Functions

- [ ] Support function declarations (other than `main`) and function calls.
- [ ] Implement the ABI defined in Phase 0.
  - [ ] Prologue: Save frame pointer, allocate stack space.
  - [ ] Epilogue: Restore frame pointer, return.
  - [ ] Argument passing logic.

---

## 🚀 Phase 2: Advanced Types

*Goal: Implement selected chapters from Part II (Types beyond `int`). Note: Chapters 11 (Longs) and 12 (Unsigned) are skipped.*

### Chapter 13: Floating-Point Numbers

- [ ] Add `double` type to Lexer/Parser.
- [ ] Map to native floating-point Simuproc instructions.

### Chapter 14: Pointers

- [ ] Support `*` (dereference) and `&` (address-of).
- [ ] Update Type Checker to validate pointer types.
- [ ] Code Gen: Implement indirect addressing modes (e.g., `MOV AX, [BX]`).

### Chapter 15: Arrays & Pointer Arithmetic

- [ ] Support array declaration `int a[10]`.
- [ ] Implement pointer arithmetic (scaling by type size).
- [ ] Support subscript operator `[]`.

### Chapter 16: Characters & Strings

- [ ] Support `char` type and string literals `"hello"`.
- [ ] Code Gen: distinct handling for byte-sized operations vs word-sized operations.
- [ ] Implement basic I/O mapping to Simuproc I/O ports.
