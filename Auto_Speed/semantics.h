#pragma once
#include "scanner.h"
#include <string>
#include <unordered_map>
#include <vector>

class SemanticAnalyzer {
private:
    bool hasErr = false;

    // scope stack: each scope is a map name -> type
    std::vector<std::unordered_map<std::string, std::string>> scopes;

    // functions remain in one global table
    std::unordered_map<std::string, int> funcs;

    void reportError(const std::string& msg, const Token& t);

public:
    SemanticAnalyzer();

    bool hasErrors() const;

    // scope management
    void enterScope();
    void leaveScope();

    // variables
    void declareVariable(const std::string& name, const std::string& type, const Token& t);
    void assignVariable(const std::string& name, const Token& t);
    bool isDeclaredInCurrentScope(const std::string& name) const;

    // functions
    void declareFunction(const std::string& name, int paramCount, const Token& t);
    void callFunction(const std::string& name, int argCount, const Token& t);
};
