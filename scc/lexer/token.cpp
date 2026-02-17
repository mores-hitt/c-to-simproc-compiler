#include "lexer/token.h"

#include <string_view>
#include <iostream>
#include <optional>
#include <algorithm>
#include <cctype>

namespace scc {

    namespace {

        template<typename MapType, typename KeyType> scc::Token Map(
            const KeyType& lookupKey,
            std::string_view tokenValue,
            int lineNumber,
            int columnNumber,
            const MapType& map,
            const char* categoryName,
            TokenType tokenFallback) {
            
            auto it = map.find(lookupKey);
            
            if (it != map.end()) {
                return Token{it->second, tokenValue, lineNumber, columnNumber};
            } else {
                std::cerr << "Warning: unknown " << categoryName << " '" << tokenValue << "' on line " << lineNumber << "\n";
                return Token{tokenFallback, tokenValue, lineNumber, columnNumber};
            }
        }

    }

    scc::Token makeDelimiterToken(const char* c, int lineNumber, int columnNumber) {
        return Map(*c, std::string_view(c, 1), lineNumber, columnNumber, delimiterMap, "delimiter", TokenType::undefined);
    }


    scc::Token makeKeywordToken(std::string_view word, int lineNumber, int columnNumber) {
        return Map(word, word, lineNumber, columnNumber, keywordMap, "keyword", TokenType::identifier);
    }

}