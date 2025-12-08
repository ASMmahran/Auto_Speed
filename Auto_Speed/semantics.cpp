#include "semantics.h"
#include <iostream>

using namespace std;

SemanticAnalyzer::SemanticAnalyzer() : hasErr(false) {
}

bool SemanticAnalyzer::hasErrors() const {
    return hasErr;
}

void SemanticAnalyzer::reportError(const string& msg, const Token& t) {
    hasErr = true;
    cerr << "Semantic Error [Line " << t.line << "]: " << msg << " (near '" << t.value << "')" << endl;
}

void SemanticAnalyzer::declareVariable(const string& name, const string& type, const Token& t) {
    if (vars.count(name)) {
        reportError("Variable '" + name + "' already declared", t);
        return;
    }
    vars[name] = type;
}

void SemanticAnalyzer::assignVariable(const string& name, const Token& t) {
    if (!vars.count(name)) {
        reportError("Variable '" + name + "' used before declaration", t);
    }
}

void SemanticAnalyzer::declareFunction(const string& name, int paramCount, const Token& t) {
    if (funcs.count(name)) {
        reportError("Function '" + name + "' already declared", t);
        return;
    }
    funcs[name] = paramCount;
}

void SemanticAnalyzer::callFunction(const string& name, int argCount, const Token& t) {
    if (!funcs.count(name)) {
        reportError("Call to undefined function '" + name + "'", t);
        return;
    }
    if (funcs[name] != argCount) {
        reportError("Function '" + name + "' expects " + to_string(funcs[name]) + " args, got " + to_string(argCount), t);
    }
}
