#include "lexer/token.h"

#include <string_view>
#include <iostream>
#include <optional>
#include <algorithm>
#include <cctype>

namespace scc {

    namespace {

        template<typename MapType, typename KeyType> scc::Token Map(
            const KeyType& lookupKey, std::string_view tokenValue,
            int lineNumber, const MapType& map, const char* categoryName,
            TokenType tokenFallback) {
            
            auto it = map.find(lookupKey);
            
            if (it != map.end()) {
                return Token{it->second, tokenValue, lineNumber};
            } else {
                std::cerr << "Warning: unknown " << categoryName << " '" << tokenValue << "' on line " << lineNumber << "\n";
                return Token{tokenFallback, tokenValue, lineNumber};
            }
        }

    }

    scc::Token isDelimiter(const char* c, int lineNumber) {
        return Map(*c, std::string_view(c, 1), lineNumber, delimiterMap, "delimiter", TokenType::undefined);
    }

    bool isDelimiter(char c){
        return delimiterMap.count(c) > 0;
    }


    scc::Token isKeyword(std::string_view word, int lineNumber) {
        return Map(word, word, lineNumber, keywordMap, "keyword", TokenType::identifier);
    }

}