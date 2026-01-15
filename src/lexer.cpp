#include "scc/lexer.h"
#include "scc/token.h"

#include <vector>
#include <string_view>
#include <iostream>
#include <cctype>
#include <algorithm>
// I need to keep track of many things
// line number
// previous white space, to recoginze keywords and stuff
// literal flag (could this be a literal?)
// so, it screams that, maybe, i need a bunch of variables before the loop
// or, i can make a struct or a class

namespace scc {

    std::vector<Token> lexical_analysis(std::string_view sourceCode) {

        const char* begin {sourceCode.data()};
        const char* end {sourceCode.data() + sourceCode.size()};
        int lineNumber {1};
        int columnNumber {1};
        const char* tokenStart {nullptr};
        const char* tokenEnd {nullptr};

        std::vector<Token> tokenVector {};

        tokenVector.reserve(sourceCode.size() / 4); // 1 token per 4 characters estimate
        
        for (auto p {begin}; p != end; p++){

            std::cout << "line number: " << lineNumber << ". ";
            std::cout << "column/character number: " << columnNumber << ". ";

            if (*p == '\n') {

                std::cout << "here, a new line: \\n " << "\n";
                lineNumber++;
                columnNumber = 1;
                continue;

            } else if (std::isalpha(*p)){

                std::cout << "start of token \n";
                tokenStart = p;
                while (p != end && !std::isspace(*p) && !scc::isDelimiter(*p)) { // keep looping until break
                    p++;
                    columnNumber++;
                }
                std::cout << "end of token: ";
                tokenEnd = p;

                std::string_view tokenValue(tokenStart, static_cast<size_t>(tokenEnd - tokenStart)); // get size of token by pointer substraction

                std::cout << tokenValue << "\n";

                tokenVector.push_back(scc::isKeyword(tokenValue, lineNumber));

                p--; // while loops leaves p one character forward

                continue;

            } else if (std::isdigit(*p)) {

                std::cout << "start of integer constant?\n";
                tokenStart = p;
                while ( p != end && std::isdigit(*p)) { // keep looping until no more digits
                    p++;
                    columnNumber++;
                }
                if (p != end && !scc::isDelimiter(*p) && !std::isspace(*p)) {
                    std::cerr << "Warning. broken integer constant at " << lineNumber << ". Crazy behaviour incoming\n";
                }

                std::cout << "end of integer literal: ";
                tokenEnd = p;
                std::string_view tokenValue(tokenStart, static_cast<size_t>(tokenEnd- tokenStart)); // get size of token by pointer substraction

                std::cout << tokenValue << "\n";

                Token token {TokenType::integer_constant, tokenValue, lineNumber};

                tokenVector.push_back(token);

                p--; // while loops leaves p one character forward

                continue;

            } else if (std::isspace(*p)) {

                std::cout << "here, a whitespace " << *p << "\n";
                columnNumber++;
                
                continue;

            } else {

                std::cout << "here, a delimiter?: " << *p << "\n";
                tokenVector.push_back(scc::isDelimiter(p, lineNumber));
                columnNumber++;

            }

        }


        return tokenVector;
    }

}