#pragma once

#include "scc/token.h"
#include <string_view>
#include <vector>

namespace scc{

    std::vector<Token> lexical_analysis(std::string_view sourceCode);

}