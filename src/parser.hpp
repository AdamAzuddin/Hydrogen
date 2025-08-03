#pragma once
#include <vector>
#include <iostream>
#include <optional>
#include <variant>
#include "tokenization.hpp"

struct NodeExprIntLit
{
    Token int_lit;
};

struct NodeExprIdent
{
    Token ident;
};

struct NodeExpr
{
    std::variant<NodeExprIntLit, NodeExprIdent> var;
};

struct NodeStatLet
{
    Token ident;
    NodeExpr expr;
};

struct NodeStatExit
{
    NodeExpr expr;
};

struct NodeStat
{
    std::variant<NodeStatExit, NodeExprIdent, NodeStatLet> var;
};

struct NodeProg
{
    std::vector<NodeStat> stats;
};

class Parser
{
public:
    inline explicit Parser(std::vector<Token> tokens) : m_tokens(std::move(tokens)) {

                                                        };

    std::optional<NodeExpr> parse_expr()
    {
        if (peek().has_value() && peek().value().type == TokenType::int_lit)
        {
            return NodeExpr{.var = NodeExprIntLit{.int_lit = consume()}};
        }
        if (peek().has_value() && peek().value().type == TokenType::ident)
        {
            return NodeExpr{.var = NodeExprIdent{.ident = consume()}};
        }
        else
        {
            return {};
        }
    }

    std::optional<NodeStat> parse_stat()
    {
        if (peek().has_value() && peek().value().type == TokenType::_exit && peek(1).has_value() && peek(1).value().type == TokenType::open_paren)
        {
            consume();
            consume();
            NodeStatExit stat_exit;
            if (auto node_expr = parse_expr())
            {
                stat_exit = {.expr = node_expr.value()};
            }
            else
            {
                std::cerr << "Invalid expression" << std::endl;
                exit(EXIT_FAILURE);
            }

            if (peek().has_value() && peek().value().type == TokenType::close_paren)
            {
                consume();
            }
            else
            {
                std::cerr << "Expected ')'" << std::endl;
                exit(EXIT_FAILURE);
            }
            if (peek().has_value() && peek().value().type == TokenType::semi)
            {
                consume();
            }
            else
            {
                std::cerr << "Expected ';'" << std::endl;
                exit(EXIT_FAILURE);
            }
            return NodeStat{.var = stat_exit};
        }
        else if (peek().has_value() && peek().value().type == TokenType::let && peek(1).has_value() && peek(1).value().type == TokenType::ident && peek(2).has_value() && peek(2).value().type == TokenType::eq)
        {
            consume();
            auto stat_let = NodeStatLet{.ident = consume()};
            consume();
            if (auto expr = parse_expr())
            {
                stat_let.expr = expr.value();
            }
            else
            {
                std::cerr << "Invalid expression" << std::endl;
                exit(EXIT_FAILURE);
            }

            if (peek().has_value() && peek().value().type == TokenType::semi)
            {
                consume();
            }
            else
            {
                std::cerr << "Expected ';'" << std::endl;
                exit(EXIT_FAILURE);
            }

            return NodeStat{.var = stat_let};
        }
        else
        {
            return {};
        }
    }

    std::optional<NodeProg> parse_prog()
    {
        NodeProg prog;
        while (peek().has_value())
        {
            if (auto stat = parse_stat())
            {
                prog.stats.push_back(stat.value());
            }
            else
            {
                std::cerr << "Invalid statement" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        return prog;
    }

private:
    [[nodiscard]] std::optional<Token> peek(int ahead = 0) const
    {
        if (m_index + ahead >= m_tokens.size())
        {
            return {};
        }
        else
        {
            return m_tokens.at(m_index + ahead);
        }
    };

    Token consume()
    {
        return m_tokens.at(m_index++);
    };
    const std::vector<Token> m_tokens;
    size_t m_index = 0;
};