#include "ASTPrinter.hh"
#include "ASTNode.hh"
#include <iostream>
#include <sstream>
#include <vector>

void ASTPrinter::printIndent()
{
    for (int i = 0; i < indent_level; ++i)
    {
        std::cout << "  "; // Zwei Leerzeichen pro Ebene
    }
}

std::string printRetrunType(const Type &t)
{
    switch (t)
    {
    case Type::INTEGER:
        return "zahl";
    case Type::STRING:
        return "faden";
    case Type::BOOLEAN:
        return "bool";
    case Type::VOID:
        return "leer";
    default:
        return "unbekannt";
    }
}

std::string getNodeTypeName(const ASTNodeType &t)
{
    switch (t)
    {
    case ASTNodeType::NUMBER:
        return "Zahl";
    case ASTNodeType::STRING:
        return "Text";
    case ASTNodeType::VAR_REF:
        return "Variablen-Referenz";
    case ASTNodeType::BINARY_OP:
        return "Operation";
    case ASTNodeType::VAR_DECL:
        return "Deklaration";
    case ASTNodeType::RETURN:
        return "Rückgabe";
    case ASTNodeType::BLOCK:
        return "Block";
    case ASTNodeType::FUNCTION_DEF:
        return "Funktion";
    case ASTNodeType::UNIT:
        return "Einheit";
    default:
        return "Knoten";
    }
}

std::string getOpTypeName(const OperationType &op)
{
    switch (op)
    {
    case OperationType::ADD:
        return "+";
    case OperationType::SUBTRACT:
        return "-";
    case OperationType::MULTIPLY:
        return "*";
    case OperationType::DIVIDE:
        return "/";
    case OperationType::ASSIGN:
        return "=";
    case OperationType::MODULO:
        return "%";
    case OperationType::EQ:
        return "==";
    case OperationType::NEQ:
        return "!=";
    case OperationType::LESS:
        return "<";
    case OperationType::LEQ:
        return "<=";
    case OperationType::GEQ:
        return ">=";
    case OperationType::GREATER:
        return ">";
    default:
        return "unbekannte Operation";
    }
}

void ASTPrinter::visit(UnitNode *rootNode)
{
    for (const auto &func : rootNode->functions)
    {
        visit(func.get());
        std::cout << std::endl;
    }
}

std::stringstream printFunctionParameters(const std::vector<FunctionParameter> &params)
{
    std::stringstream ss;
    for (const auto &param : params)
    {
        ss << param.identifier << ": " << printRetrunType(param.type);
        if (&param != &params.back())
        {
            ss << ", ";
        }
    }
    return ss;
}

void ASTPrinter::visit(FunctionDefNode *node)
{
    printIndent();
    std::cout << "| FUNKTION " << node->name << "(" << printFunctionParameters(node->parameters).str() << ")" << " -> " << printRetrunType(node->return_type) << std::endl;
    if (node->body)
    {
        node->body->accept(*this);
    }
}

void ASTPrinter::visit(FunctionCallNode *node)
{
    printIndent();
    std::cout << "| FUNKTION_CALL: " << node->function_name << std::endl;
    if (node->params.empty())
        return;

    indent_level++;
    for (const auto &param : node->params)
    {
        param->accept(*this);
    }
    indent_level--;
}

void ASTPrinter::visit(BlockNode *node)
{
    printIndent();
    indent_level++;
    std::cout << "| BLOCK BEGIN" << std::endl;
    for (const auto &stmt : node->statements)
    {
        stmt->accept(*this);
    }
    indent_level--;
}

void ASTPrinter::visit(ReturnNode *node)
{
    printIndent();
    std::cout << "| ZURÜCK" << std::endl;
    if (node->expression)
    {
        indent_level++;
        node->expression->accept(*this);
        indent_level--;
        printIndent();
    }
    std::cout << std::endl;
}

void ASTPrinter::visit(WhileNode *node)
{
    printIndent();
    std::cout << "| SOLANGE" << std::endl;
    indent_level++;
    printIndent();
    std::cout << "BEDINGUNG:" << std::endl;
    indent_level++;
    node->condition->accept(*this);
    indent_level--;
    printIndent();
    std::cout << "KÖRPER:" << std::endl;
    indent_level++;
    node->body->accept(*this);
    indent_level -= 2;
}

void ASTPrinter::visit(VarDeclNode *node)
{
    printIndent();
    std::cout << "| VARIABLE " << node->name << " : " << printRetrunType(node->type);
    if (node->init_value)
    {
        std::cout << " =" << std::endl;
        indent_level++;
        node->init_value->accept(*this);
        indent_level--;
    }
    else
    {
        std::cout << std::endl;
    }
}

void ASTPrinter::visit(BinaryOpNode *node)
{
    printIndent();
    std::cout << "| OP [" << getOpTypeName(node->op) << "]" << std::endl;

    indent_level++;
    if (node->left)
        node->left->accept(*this);
    if (node->right)
        node->right->accept(*this);
    indent_level--;
}

void ASTPrinter::visit(VarRefNode *node)
{
    printIndent();
    std::cout << "| REF: " << node->name << std::endl;
}

void ASTPrinter::visit(StringNode *node)
{
    printIndent();
    std::cout << "| STR: \"" << node->value << "\"" << std::endl;
}

void ASTPrinter::visit(NumberNode *node)
{
    printIndent();
    std::cout << "| NUM: " << node->value << std::endl;
}

void ASTPrinter::visit(BoolNode *node)
{
    printIndent();
    std::cout << "| BOOL: " << node->value << std::endl;
}