#pragma once
#include "../ast/ASTVisitor.hh"
#include "../symboltable/Symboltable.hh"
#include "../ast/ASTNode.hh"
#include "../symboltable/FunctionSymboltable.hh"
#include <stack>
#include <memory>
class SemanticChecker : public ASTVisitor {
public:
    SemanticChecker(const FunctionSymboltable& functionTable) : functionTable(functionTable) {}
    std::shared_ptr<Symboltable> getCurrentVariableSymbolTable();
    
    void visit(UnitNode* rootNode) override;
    void visit(FunctionDefNode* node)override;
    void visit(FunctionCallNode* node) override;
    void visit(BlockNode* node) override;
    void visit(ReturnNode* node) override;
    void visit(VarDeclNode* node) override;
    void visit(BinaryOpNode* node) override;
    void visit(VarRefNode* node) override;
    void visit(StringNode* node) override;
    void visit(NumberNode* node) override;

private:
    std::shared_ptr<Symboltable> rootSymbolTable;
    std::stack<Symboltable*> symbolTableStack;   
    const FunctionSymboltable& functionTable;   

    bool addFunctionParametersToTable(const FunctionDefNode &node, Symboltable &symbolTable);
    bool functionBodyMode = false;
    Symboltable* constructNewSymbolTable();

    Type current_exp_type;
    void validateBinaryOpCalcTypes(const BinaryOpNode* node, const Type& leftType, const Type& rightType);
    void validateBinaryOpTypes(const BinaryOpNode* node, const Type& leftType, const Type& rightType);
};