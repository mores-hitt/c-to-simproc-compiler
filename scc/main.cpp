#include "lexer/lexer.h"

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <cstddef>
#include <optional>
#include <string_view>

std::string getSourceCode(std::string_view path) {
    
    const std::filesystem::path file_path {path};
    
    std::cout << file_path << "\n";
    
    if (!std::filesystem::exists(file_path)) {
        std::cerr << "File does not exist at: " << file_path << "\n";
        std::cerr << "Usage: scc <source_file.c>\n";
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

    return sourceCode;
}

int main (int argc, char* argv[]) {

    #ifdef DEBUG_MODE
    std::cout << "Running in DEBUG mode!\n";
    #endif

    if (argc < 2) {
        std::cerr << "Usage: scc <source_file.c>\n";
        return 1;
    } else if (argc > 2) {
        std::cerr << "Error. No more than one argument\n";
        std::cerr << "Usage: scc <source_file.c>\n";
        return 1;
    }

    auto sourceCode = getSourceCode(argv[1]);

    if (sourceCode == "") {
        std::cerr << "Error. No source code found\n";
        std::cerr << "Usage: scc <source_file.c>\n";
        return 1;
    }

    auto lexer = scc::Lexer(sourceCode);

    try
    {
        auto vec = lexer.analize();
    
        std::cout << "\n\n###################################################################\n\n";
    
        for (size_t i = 0; i < vec.size(); i++)
        {
            std::cout << "Token: " << vec.at(i).value << "\n";
            std::cout << "Line: " << vec.at(i).lineNumber << "\n";
            std::cout << "Type: " << vec.at(i).type << "\n\n\n";
        }
    
        return 0;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
    

}