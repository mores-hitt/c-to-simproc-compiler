#include "errors/diagnostic.h"
#include <iostream>

namespace scc {
    void Diagnostics::report() {
        for (const auto& d : m_diagnostics) {
            std::cerr << (d.severity == scc::Diagnostic::Severity::Error ? "error" : "warning")
                      << " at line " << d.location.line << ", col " << d.location.column
                      << ": " << d.message << '\n';
        }
    }
}