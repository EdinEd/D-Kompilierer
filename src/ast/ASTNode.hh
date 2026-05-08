#ifndef ASTNODE_HH
#define ASTNODE_HH

class ASTNode
{
public:
    ASTNode() = default;
    virtual ~ASTNode() = default;
    virtual void print() const = 0;
};
#endif // ! ASTNODE_HH