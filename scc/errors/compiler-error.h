#pragma once

#include <string>
#include <stdexcept>

namespace scc {

    struct SourceLocation {
        int line;
        int column;
    };

    class CompilerError : public std::runtime_error {
    public:
        SourceLocation location;

        CompilerError(const std::string& message, SourceLocation loc)
            : std::runtime_error(message)
            , location(loc) {}
    };

    class LexerError : public CompilerError {
        using CompilerError::CompilerError; // inherit constructor
    };

    class ParseError : public CompilerError {
        using CompilerError::CompilerError;
    };

    class CodegenError : public CompilerError {
        using CompilerError::CompilerError;
    };

} // namespace scc