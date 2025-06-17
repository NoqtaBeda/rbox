/**
 * Copyright (c) 2026 NoqtaBeda (noqtabeda@163.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 **/

#include <cassert>
#include <format>
#include <print>
#include <rbox/enum/enum_cast.hpp>
#include <rbox/enum/enum_name.hpp>
#include <rbox/lookup.hpp>
#include <rbox/utils/ctype/case_conversion.hpp>  // ascii_toupper

enum class Opcode {
    INVALID = 0,
    CONSTANT,
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    NEG,
    AND,
    OR,
    XOR,
    NOT,
};

struct Node {
    Opcode opcode;
    int index;
};

struct ConstantNode : Node {
    int value;
};

struct UnaryNode : Node {
    Node* operand;
};

struct BinaryNode : Node {
    Node* lhsOperand;
    Node* rhsOperand;
};

struct NodeBuilder {
    int index = 0;

    auto Constant(int value) -> ConstantNode*
    {
        return new ConstantNode({Opcode::CONSTANT, index++}, value);
    }

    auto Add(Node* lhs, Node* rhs) -> BinaryNode*
    {
        assert(lhs != nullptr);
        assert(rhs != nullptr);
        return new BinaryNode{{Opcode::ADD, index++}, lhs, rhs};
    }

    auto Sub(Node* lhs, Node* rhs) -> BinaryNode*
    {
        assert(lhs != nullptr);
        assert(rhs != nullptr);
        return new BinaryNode{{Opcode::SUB, index++}, lhs, rhs};
    }

    auto Mul(Node* lhs, Node* rhs) -> BinaryNode*
    {
        assert(lhs != nullptr);
        assert(rhs != nullptr);
        return new BinaryNode{{Opcode::MUL, index++}, lhs, rhs};
    }

    auto Div(Node* lhs, Node* rhs) -> BinaryNode*
    {
        assert(lhs != nullptr);
        assert(rhs != nullptr);
        return new BinaryNode{{Opcode::DIV, index++}, lhs, rhs};
    }

    auto Mod(Node* lhs, Node* rhs) -> BinaryNode*
    {
        assert(lhs != nullptr);
        assert(rhs != nullptr);
        return new BinaryNode{{Opcode::MOD, index++}, lhs, rhs};
    }

    auto Neg(Node* operand) -> UnaryNode*
    {
        assert(operand != nullptr);
        return new UnaryNode{{Opcode::NEG, index++}, operand};
    }

    auto And(Node* lhs, Node* rhs) -> BinaryNode*
    {
        assert(lhs != nullptr);
        assert(rhs != nullptr);
        return new BinaryNode{{Opcode::AND, index++}, lhs, rhs};
    }

    auto Or(Node* lhs, Node* rhs) -> BinaryNode*
    {
        assert(lhs != nullptr);
        assert(rhs != nullptr);
        return new BinaryNode{{Opcode::OR, index++}, lhs, rhs};
    }

    auto Xor(Node* lhs, Node* rhs) -> BinaryNode*
    {
        assert(lhs != nullptr);
        assert(rhs != nullptr);
        return new BinaryNode{{Opcode::XOR, index++}, lhs, rhs};
    }

    auto Not(Node* operand) -> UnaryNode*
    {
        assert(operand != nullptr);
        return new UnaryNode{{Opcode::NOT, index++}, operand};
    }

    auto BuildArithmetic(Opcode opcode, Node* lhs, Node* rhs) -> Node*
    {
        // How does it work:
        // For each addressable member, its reflection is passed to the transformation function.
        // If the transformation result R != std::nullopt, then we take R.value() as the key.
        // Otherwise, this member is ignored.

        // Invokes overload (3.1):
        // rbox::make_static_member_function_fixed_map(T, transform_fn, options, ctx)
        constexpr auto DISPATCH_TABLE = RBOX_NONSTATIC_MEMBER_FUNCTION_FIXED_MAP(
            // 1st arg: The class whose members are to be traversed
            NodeBuilder,
            // 2nd arg: The transform function with call signature
            // (std::meta::info) -> std::optional<Opcode>
            [](std::meta::info member) -> std::optional<Opcode> {
                if (member == ^^NodeBuilder::BuildArithmetic) {
                    return std::nullopt;  // Filters out BuildArithmetic() itself
                }
                if (member == ^^NodeBuilder::Constant) {
                    return std::nullopt;  // Filters out Constant()
                }
                if (!has_identifier(member)) {
                    return std::nullopt;  // Identifier check
                }
                // Returns std::nullopt if identifier_of(member) is not one of Opcode
                auto upperCase = rbox::ascii_toupper(identifier_of(member));
                return rbox::enum_cast<Opcode>(upperCase);
            }
            // 3rd arg (optional): Configuration of the underlying fixed map.

            // 4th arg (optional): Access context, std::meta::access_context::current() by default.
        );
        // Value type is (order of alternative types are unspecified):
        // rbox::meta_variant<
        //   UnaryNode *(NodeBuilder::*)(Node *),
        //   BinaryNode *(NodeBuilder::*)(Node *, Node *)>

        return DISPATCH_TABLE[opcode].visit([this, lhs, rhs](auto mptr) -> Node* {
            if (mptr == nullptr) {
                return nullptr;  // On invalid opcode
            }
            if constexpr (requires { (this->*mptr)(lhs, rhs); }) {
                return (this->*mptr)(lhs, rhs);  // Binary
            } else {
                return (this->*mptr)(lhs);  // Unary
            }
        });
    }
};

auto DumpToExpr(const Node* node) -> std::string
{
    // Constant
    if (node->opcode == Opcode::CONSTANT) {
        return std::to_string(static_cast<const ConstantNode*>(node)->value);
    }
    // Unary
    if (node->opcode == Opcode::NEG || node->opcode == Opcode::NOT) {
        auto* asUnary = static_cast<const UnaryNode*>(node);
        return std::format("{}({})", rbox::enum_name(node->opcode), DumpToExpr(asUnary->operand));
    }
    // Binary
    auto* asBinary = static_cast<const BinaryNode*>(node);
    return std::format(
        "{}({}, {})",
        rbox::enum_name(node->opcode),
        DumpToExpr(asBinary->lhsOperand),
        DumpToExpr(asBinary->rhsOperand));
}

int main()
{
    NodeBuilder builder;

    ConstantNode* a10 = builder.Constant(10);
    ConstantNode* a4 = builder.Constant(4);
    ConstantNode* a7 = builder.Constant(7);

    // d = (~(10 | 7)) + 4
    Node* b = builder.BuildArithmetic(Opcode::XOR, a10, a7);
    Node* c = builder.BuildArithmetic(Opcode::NEG, b, nullptr);
    Node* d = builder.BuildArithmetic(Opcode::ADD, c, a4);

    // d = ADD(NEG(XOR(10, 7)), 4)
    std::println("d = {}", DumpToExpr(d));
    return 0;
}
