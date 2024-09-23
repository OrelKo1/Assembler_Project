# Assembler Program - Systems Programming Lab

## Project Overview
This project is an assembler implemented in C, developed as part of the Systems Programming Lab course. The assembler translates assembly code written for a hypothetical CPU into machine code, supporting various addressing methods, data definitions, and error detection mechanisms.

The project follows a two-pass compilation process, with macro expansion, binary code translation, and the generation of machine-readable output files.

## Features
- **Macro Expansion**: The assembler includes a pre-assembler step that processes macros, mapping them into a key-value structure. The key is the macro name, and the value is a list of lines representing the macro’s definition.
- **Instruction and Directive Encoding**: The program encodes assembly instructions and directives into binary using a custom format. Instructions are validated, and metadata such as operand addressing methods and opcode information are extracted.
- **Error Detection**: The assembler performs validation for each line of code, ensuring that the syntax and structure meet the requirements. If errors are detected, appropriate messages are printed.
- **Binary Code Generation**: The first pass translates the instructions into a 12-bit binary code. For tag-based addressing, final indexation occurs in the second pass. The output file is formatted in base64.

## Project Structure
The assembler runs in several stages:

1. **Macro Processing**: The program processes all macros in the input `.as` file, expanding them and generating a new `.am` file with the expanded content.
   
2. **First Pass**:
   - Analyze each line of code using the `analyzeLine` function, which extracts opcode, operand types, and values.
   - Validate the instruction using the `validateData` function, ensuring the line adheres to the correct syntax.
   - Convert valid instructions into binary using the `fillFirstPass` function, encoding the instruction into a 12-bit format.
   - Addressing for labels is deferred to the second pass for final resolution.

3. **Second Pass**:
   - The program processes the label-based operands that were deferred in the first pass.
   - Labels are indexed and resolved in this stage, and binary code is updated accordingly.

4. **Output Generation**: Finally, the binary representation of the code is saved in an `.ob` file in base64 format.

## File Output
- **Object File (`.ob`)**: Contains the binary machine code, encoded in base64.
- **Additional Output**: Macros expanded into a `.am` file, as well as error logs if any validation fails.

## Usage
To compile and run the assembler:
1. Compile the code using the following flags to ensure strict C standard adherence:
   ```bash
   gcc -Wall -ansi -pedantic -o assembler assembler.c
   ```
2. Run the assembler on an assembly input file:
   ```bash
   ./assembler filename.as
   ```

## Dependencies
The project is implemented in C and is compatible with Linux environments such as Ubuntu. Make sure `gcc` is installed for compilation.