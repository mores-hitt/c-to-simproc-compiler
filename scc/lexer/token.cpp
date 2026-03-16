#include "lexer/token.h"

#include <string_view>
#include <cctype>

namespace scc::lexer {

    scc::lexer::Token makeDelimiterToken(std::string_view tokenValue, int lineNumber, int columnNumber) {
        auto it = delimiterMap.find(tokenValue[0]);
        
        if (it != delimiterMap.end()) {
            return Token{it->second, tokenValue, lineNumber, columnNumber};
        } else {
            return Token{TokenType::undefined, tokenValue, lineNumber, columnNumber};
        }
    }


    scc::lexer::Token makeKeywordOrIdentifierToken(std::string_view word, int lineNumber, int columnNumber) {
        auto it = keywordMap.find(word);
        
        if (it != keywordMap.end()) {
            return Token{it->second, word, lineNumber, columnNumber};
        } else {
            return Token{TokenType::identifier, word, lineNumber, columnNumber};
        }
    }

}