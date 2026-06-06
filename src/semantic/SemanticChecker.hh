#pragma once
#include "../ast/ASTVisitor.hh"
#include "../symboltable/Symboltable.hh"
#include "../ast/ASTNode.hh"
#include "../symboltable/FunctionSymboltable.hh"
#include "SemanticExceptions.hh"
#include <stack>
#include <vector>
#include <memory>


class SemanticChecker : public ASTVisitor {
public:
    SemanticChecker(const FunctionSymboltable& functionTable) : functionTable(functionTable) {}
    std::shared_ptr<Symboltable> getCurrentVariableSymbolTable();
    bool hasErrors() const;
    void printErrors() const;
    
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
    bool functionBodyHasReturn = false;
    Type functionReturnType;

    Symboltable* constructNewSymbolTable();

    Type current_exp_type;
    void validateBinaryOpCalcTypes(const SourceLocation &location, const Type& leftType, const Type& rightType);
    void validateBinaryOpTypes(const SourceLocation &location, const Type& leftType, const Type& rightType);
    void validateFuncDeclCorrectness(const Type& returnType, const bool& hasReturn);
    bool validateFuncSignature(const FunctionCallNode* node, const FunctionSignature* funcSignature = nullptr);

    std::vector<SemanticError> errors;
};