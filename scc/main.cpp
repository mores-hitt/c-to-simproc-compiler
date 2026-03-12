#include "lexer/lexer.h"
#include "parser/parser.h"
#include "assembly-gen/assembly-gen.h"
#include "assembly-gen/visitors/ast-printer.h"
#include "code-emission/linux/emitter.h"
#include "parser/visitors/printer.h"

#include <CLI11.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <cstddef>
#include <string_view>
#include <cstdlib>

std::string getSourceCode(std::string_view path) {
    
    const std::filesystem::path file_path {path};
    
    // std::cerr << file_path.c_str() << "\n"; //non utf 8 for some reason
    
    if (!std::filesystem::exists(file_path)) {
        //std::cerr << "File does not exist at: " << file_path.c_str() << "\n"; //non utf 8 for some reason
        return {};
    }
    
    std::ifstream input_file{ file_path };
    
    if (!input_file)
    {
        //std::cerr << "Uh oh, not be opened for reading!\n"; //non utf 8 for some reason
        return {};
    }
    
    std::size_t fileSize {std::filesystem::file_size(file_path)};
    
    std::string sourceCode(fileSize, '\0');
    input_file.read(sourceCode.data(), static_cast<std::streamsize>(fileSize));

    return sourceCode;
}

std::string preprocess(const std::string& filePath) {

    std::filesystem::path p(filePath);

    std::string outputPath {p.replace_extension(".i")};

    std::string stringCommand {"gcc -E -P " + filePath + " -o " + outputPath};

    const char* command {stringCommand.c_str() };

    int result = std::system(command);

    if (result) {
        std::cerr << "\nPreprocessing error\n";
        throw std::runtime_error("Preprocessing error");
    }

    return outputPath;
}

void assembleAndLink(const std::filesystem::path& filePath) {
    std::string path {filePath.string()};

    std::filesystem::path outputPath = filePath;
    outputPath.replace_extension("");
    std::string outputFileName {outputPath.string()};

    std::cerr << path << '\n';
    std::cerr << outputFileName << '\n';

    std::string stringCommand {"gcc " + path + " -o " + outputFileName};
    const char* command {stringCommand.c_str()};

    std::cerr << stringCommand << '\n';

    int result = std::system(command);

    if (result) {
        std::cerr << "\nAssembling/Linking error\n";
        throw std::runtime_error("Assembling/Linking error");
    }
}

void removeFile(std::string_view filePath) {
    std::filesystem::path p{filePath};
    std::error_code ec;
    if (!std::filesystem::remove(p, ec)) {
        std::cerr << "\nFile removal error: " << ec.message() << "\n";
        throw std::runtime_error("File removal error");
    }
}

int main (int argc, char **argv) { 

    #ifdef DEBUG_MODE
    std::cerr << "Running in DEBUG mode!\n";
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
     
    try
    {
        
        std::string fileName {preprocess(filePath)};
        
        std::string sourceCode {getSourceCode(fileName)};
        
        if (sourceCode == "") {
            std::cerr << "Error. No source code found\n";
            std::cerr << "Usage: scc <source_file.c>\n";
            return 1;
        }

        removeFile(fileName);

        scc::lexer::Lexer lexer(sourceCode);

        auto vec = lexer.analyze();

        #ifdef DEBUG_MODE

        std::cerr << "\n\n#### Tokens #####\n\n";

        for (auto& token : vec) {
            std::cerr << "Token: " << token.value << '\n'
                      << "Line: " << token.lineNumber << '\n'
                      << "Column: " << token.columnNumber << '\n'
                      << "Type: " << token.type << "\n\n";
        }

        #endif

        if (lexerStage) {
            return 0;
        }

        auto parser {scc::parser::Parser(vec)};

        #ifdef DEBUG_MODE

        auto parserPrinter {scc::parser::Printer()};
        parser.accept(parserPrinter);

        #endif

        if (parserStage) {
            return 0;
        }

        scc::asm_gen::AssemblyGen assemblyGen;
        parser.accept(assemblyGen);

        #ifdef DEBUG_MODE

        auto assemblyGenPrinter {scc::asm_gen::ASTPrinter()};
        assemblyGen.accept(assemblyGenPrinter);

        #endif

        if (codegenStage) {
            return 0;
        }

        std::filesystem::path outputPath {filePath};
        outputPath.replace_extension(".s");

        {
            std::ofstream outFile {outputPath};
            if (!outFile) {
                std::cerr << "Error: cannot open output file '" << outputPath << "'\n";
                return 1;
            }
    
            auto emitter {scc::code_em::Emitter(outFile)};
    
            assemblyGen.accept(emitter);
        }

        assembleAndLink(outputPath);

        removeFile(outputPath.string());
        
        return 0;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }

}