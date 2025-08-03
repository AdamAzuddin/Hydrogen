#pragma once
#include <iostream>
#include <sstream>
#include <unordered_map>
#include "parser.hpp"

class Generator
{

public:
    inline Generator(NodeProg root) : m_prog(std::move(root)) {

                                      };

    void gen_expr(const NodeExpr &expr)
    {
        struct ExprVisitor
        {
            Generator *gen;
            void operator()(const NodeExprIntLit &expr_int_lit) const
            {
                gen->m_output << "    mov rax, " << std::stoi(expr_int_lit.int_lit.value.value()) << "\n";
                gen->push("rax");
            }

            void operator()(const NodeExprIdent &expr_ident) const
            {
                if (!gen->m_vars.contains(expr_ident.ident.value.value()))
                {
                    std::cerr << "Undeclared identifier: " << expr_ident.ident.value.value() << "\n";
                    exit(EXIT_FAILURE);
                }
                const auto &var = gen->m_vars.at(expr_ident.ident.value.value());
                std::stringstream offset;
                offset << "QWORD [rsp + " << (gen->m_stack_size - var.stack_loc - 1) * 8 << "]\n";
                gen->push(offset.str());
            }
        };
        ExprVisitor visitor{this};
        std::visit(visitor, expr.var);
    }

    void gen_stat(const NodeStat &stat)
    {
        struct StatVisitor
        {
            Generator *gen;
            void operator()(const NodeStatExit &stat_exit) const
            {
                gen->gen_expr(stat_exit.expr);
                gen->m_output << "    mov rax, 60\n";
                gen->pop("rdi");
                gen->m_output << "    syscall\n";
            }
            void operator()(const NodeStatLet &stat_let)
            {
                if (gen->m_vars.contains(stat_let.ident.value.value()))
                {
                    std::cerr << "Identifier already used: " << stat_let.ident.value.value() << "\n";
                    exit(EXIT_FAILURE);
                }

                gen->m_vars.insert({stat_let.ident.value.value(), Var{.stack_loc = gen->m_stack_size}});
                gen->gen_expr(stat_let.expr);
            }

            void operator()(const NodeExprIdent &expr_ident)
            {
            }
        };

        StatVisitor visitor{.gen = this};
        std::visit(visitor, stat.var);
    };

    [[nodiscard]] std::string gen_prog()
    {
        m_output << "global _start\n_start:\n";
        for (const NodeStat stat : m_prog.stats)
        {
            gen_stat(stat);
        }
        m_output << "    mov rax, 60\n";
        m_output << "    mov rdi, 0\n";
        m_output << "    syscall";
        return m_output.str();
    }

private:
    void push(const std::string &reg)
    {
        m_output << "    push " << reg << "\n";
        m_stack_size++;
    }

    void pop(const std::string &reg)
    {
        m_output << "    pop " << reg << "\n";
        m_stack_size--;
    }

    struct Var
    {
        size_t stack_loc;
    };

    const NodeProg m_prog;
    std::stringstream m_output;
    size_t m_stack_size = 0;
    std::unordered_map<std::string, Var> m_vars{};
};