#pragma once
#include "scanner.h"
#include "semantics.h"   
#include <string>
#include <vector>

using namespace std;

class Parser {
private:
    vector<Token> tokens;
    size_t pos = 0;
    int indentLevel = 0;
    bool hasError = false;

    SemanticAnalyzer semantics;   

    // Helpers
    Token current();
    void advance();
    bool match(TokenType t);
    void expect(TokenType t, const string& msg);
    void error(const string& msg);
    void printNode(const string& label);

    // Grammar rules
    void Program();
    void ImportSection();
    void NamespaceSection();
    void FunctionList();
    void Function();
    void Block();
    void StatementList();
    void Statement();
    void DeclarationStmt();
    void AssignmentStmt();
    void ConditionalStmt();
    void LoopStmt();
    void OutputStmt();
    void InputStmt();
    void ReturnStmt();
    void Expression();
    void Term();
    void Factor();

public:
    explicit Parser(const vector<Token>& t);
    void parse();

    bool semanticErrors() const {
        return semantics.hasErrors();
    }  
};
