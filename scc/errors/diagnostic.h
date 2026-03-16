// errors/diagnostics.h
#pragma once

#include <vector>
#include <cstdint>
#include <algorithm>

#include "errors/compiler-error.h"

namespace scc {

    struct Diagnostic {
        enum class Severity : uint8_t { Error, Warning };
        
        Severity severity;
        std::string message;
        SourceLocation location;
    };

    class Diagnostics {
    private:
        std::vector<Diagnostic> m_diagnostics;

    public:
        
        explicit Diagnostics()
            : m_diagnostics() {}
    
        void error(std::string message, SourceLocation loc) {
            m_diagnostics.push_back({ Diagnostic::Severity::Error, std::move(message), loc });
        }

        void warning(std::string message, SourceLocation loc) {
            m_diagnostics.push_back({ Diagnostic::Severity::Warning, std::move(message), loc });
        }

        [[nodiscard]] bool hasErrors() const {
            return std::any_of(m_diagnostics.begin(), m_diagnostics.end(),
                [](const Diagnostic& d) { return d.severity == Diagnostic::Severity::Error; });
        }

        [[nodiscard]] const std::vector<Diagnostic>& getAll() const { return m_diagnostics; }
        
        void clear() { m_diagnostics.clear(); }
        
        void report();
    };

} // namespace scc