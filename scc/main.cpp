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

std::string_view preprocess(const std::string& filePath) {

    std::filesystem::path p(filePath);

    std::string fileName {p.filename().string()};

    std::string outputFileName {p.stem().string() + ".i"};

    std::string stringCommand {"gcc -E -P " + filePath + " -o " + outputFileName};

    const char* command {stringCommand.c_str() };

    int result = std::system(command);

    if (result) {
        std::cerr << "\nPreprocessing error\n";
        throw std::runtime_error("Preprocessing error");
    }

    return outputFileName;
}

int main (int argc, char **argv) { 

    #ifdef DEBUG_MODE
    std::cout << "Running in DEBUG mode!\n";
    #endif

    CLI::App app{"SCC - Simuproc C Compiler - Compiles a subset of C for the Simuproc 1.4.2.0 CPU Emulator"};

    std::string filePath;

    bool lexerStage{false};
    bool parserStage{false};
    bool codegenStage{false};

    app.add_option("filePath", filePath, "path of C file");
 
    app.add_flag("--lex", lexerStage, "directs compiler to stop before parsing");
    app.add_flag("--parse", parserStage, "directs compiler to stop before assembly generation");
    app.add_flag("--codegen", codegenStage, "directs compiler to stop before code emission");

    CLI11_PARSE(app, argc, argv);

    auto sourceCode = getSourceCode(filePath);
 
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