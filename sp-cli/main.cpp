#include "memory/ram.h"
#include "cpu/cpu.h"
#include "register/address.h"
#include "register/general_purpose.h"

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <cstddef>
#include <algorithm>

/*
HERMANO MIO:
1. EN SIMUPROC, LO QUE SE ALMACENA EN MEMORIA SON STRINGS. EJ: LA INSTRUCCION MOV AX,100 SE GUARDA EN MEMORIA COMO 10AX,100
2. LOS REGISTROS TAMBIEN GUARDAN STRINGS DE LA MISMA FORMA QUE EN MEMORIA. 
3. NO HACER OPERACIONES ARITMETICAS HABIENDO STRINGS NO-DIGITOS EN LOS REGISTROS, UB
4. LA MIERDITA DEL MSG PUEDE GUARDAR CUALQUIER CANTIDAD DE TEXTO, NO SE VE REFLEJADO EN GASTO REAL DE MEMORIA
5. LOS STRINGS DE MSG SON DE MAXIMO 1024 DE LARGO
6. SI OCURRE UNA TRADUCCION DE OPCODE A INSTRUCCION, PERO LOS OPERANDOS PUEDEN SER STRINGS TAMBIEN
7. EL MOV NO TOCA NINGUN REGISTRO EN SU OPERACION

AUN DESCONOZCO EL COMPORTAMIENTO DE BITSHIFTS, AND, OR, ETC
*/

std::string getSourceCode(std::string_view path) {
    
    const std::filesystem::path file_path {path};
    
    std::cout << file_path << "\n";
    
    if (!std::filesystem::exists(file_path)) {
        std::cerr << "File does not exist at: " << file_path << "\n";
        return {};
    }
    
    std::ifstream input_file{ file_path };
    
    if (!input_file)
    {
        std::cerr << "Uh oh, not be opened for reading!\n";
        return {};
    }
    
    std::size_t fileSize {std::filesystem::file_size(file_path)};
    
    std::string sourceCode(fileSize, '\0');
    input_file.read(sourceCode.data(), static_cast<std::streamsize>(fileSize));

    sourceCode.erase(std::remove(sourceCode.begin(), sourceCode.end(), '\r'), sourceCode.end());
    return sourceCode;
}

int main (int argc, char* argv[]) {

    #ifdef DEBUG_MODE
    std::cout << "Running in DEBUG mode!\n";
    #endif

    if (argc < 2) {
        std::cerr << "Usage: sp-cli <source_file.txt>\n";
        return 1;
    } else if (argc > 2) {
        std::cerr << "Error. No more than one argument\n";
        std::cerr << "Usage: sp-cli <source_file.txt>\n";
        return 1;
    }

    auto sourceCode = getSourceCode(argv[1]);

    if (sourceCode == "") {
        std::cerr << "Error. No source code found\n";
        std::cerr << "Usage: sp-cli <source_file.txt>\n";
        return 1;
    }

    //sp_cli::Memory mem(sourceCode);
    //mem.print(200, 400);

    sp_cli::CPU cpu(sourceCode);
    try
    {
        cpu.run();
        cpu.printStatus();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}