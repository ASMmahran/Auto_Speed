#pragma once
#include "scanner.h"
#include <string>
#include <unordered_map>

class SemanticAnalyzer {
private:
    bool hasErr = false;
    // variable name -> type (string representation of declared type)
    std::unordered_map<std::string, std::string> vars;
    // function name -> param count
    std::unordered_map<std::string, int> funcs;

    void reportError(const std::string& msg, const Token& t);

public:
    SemanticAnalyzer();
    bool hasErrors() const;

    // variable operations
    void declareVariable(const std::string& name, const std::string& type, const Token& t);
    void assignVariable(const std::string& name, const Token& t);
    // functions
    void declareFunction(const std::string& name, int paramCount, const Token& t);
    void callFunction(const std::string& name, int argCount, const Token& t);
};
#pragma once
