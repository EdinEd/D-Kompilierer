#include "ASTPrinter.hh"
#include "ASTNode.hh"

void ASTPrinter::printIndent()
{
    for (int i = 0; i < indent_level; ++i)
    {
        std::cout << "  "; // Zwei Leerzeichen pro Ebene
    }
}

std::string printRetrunType(Type &t)
{
    switch (t)
    {
    case Type::INTEGER:
        return "ganz";
    case Type::STRING:
        return "text";
    case Type::BOOLEAN:
        return "bool";
    case Type::VOID:
        return "leer";
    default:
        return "unbekannt";
    }
}

std::string getNodeTypeName(ASTNodeType &t)
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

void ASTPrinter::visit(UnitNode *rootNode)
{
    for (const auto &func : rootNode->functions)
    {
        visit(func.get());
        std::cout << std::endl;
    }
}

void ASTPrinter::visit(FunctionDefNode *node)
{
    printIndent();
    std::cout << "| FUNKTION " << node->name << " -> " << printRetrunType(node->return_type) << std::endl;
    if (node->body)
    {
        node->body->accept(*this);
    }
}

void ASTPrinter::visit(BlockNode *node)
{
    printIndent();
    indent_level++; // Einrückung erhöhen
    std::cout << "| BLOCK BEGIN" << std::endl;
    for (const auto &stmt : node->statements)
    {
        stmt->accept(*this);
    }
    indent_level--; // Einrückung zurücksetzen

    printIndent();
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
    std::cout << "| OP [" << (int)node->op << "]" << std::endl; // Hier könntest du noch ein Op-zu-String Mapping machen

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