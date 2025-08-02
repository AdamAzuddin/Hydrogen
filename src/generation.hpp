#pragma once
#include <iostream>
#include <sstream>
#include "parser.hpp"

class Generator
{

public:
    inline Generator(NodeExit root) : m_root(std::move(root)) {

                                      };

    [[nodiscard]] std::string generate() const
    {
        std::stringstream output;
        output << "global _start\n_start:\n";
        output << "    mov rax, 60\n";
        if (!m_root.expr.int_lit.value.has_value())
        {
            std::cerr << "Missing int literal value in node" << std::endl;
            exit(EXIT_FAILURE);
        }
        output << "    mov rdi, " << m_root.expr.int_lit.value.value() << "\n";
        output << "    syscall";

        return output.str();
    }

private:
    const NodeExit m_root;
};