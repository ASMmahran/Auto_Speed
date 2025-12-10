#include "semantics.h"
#include <iostream>

using namespace std;

SemanticAnalyzer::SemanticAnalyzer() : hasErr(false) {
    // create global (outermost) scope
    scopes.emplace_back();
}

bool SemanticAnalyzer::hasErrors() const {
    return hasErr;
}

void SemanticAnalyzer::reportError(const string& msg, const Token& t) {
    hasErr = true;
    cerr << "Semantic Error [Line " << t.line << "]: " << msg << " (near '" << t.value << "')" << endl;
}

// ----- scope management -----
void SemanticAnalyzer::enterScope() {
    scopes.emplace_back();
}

void SemanticAnalyzer::leaveScope() {
    if (!scopes.empty()) scopes.pop_back();
    // keep at least the global scope if you prefer:
    // if (scopes.empty()) scopes.emplace_back();
}

// ----- variable handling -----
void SemanticAnalyzer::declareVariable(const string& name, const string& type, const Token& t) {
    if (scopes.empty()) scopes.emplace_back();

    auto& curr = scopes.back();
    if (curr.count(name)) {
        reportError("Variable '" + name + "' already declared in this scope", t);
        return;
    }
    curr[name] = type;
}

void SemanticAnalyzer::assignVariable(const string& name, const Token& t) {
    // lookup from top down
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        if (it->count(name)) return; // found
    }
    reportError("Variable '" + name + "' used before declaration (or out of scope)", t);
}

bool SemanticAnalyzer::isDeclaredInCurrentScope(const string& name) const {
    if (scopes.empty()) return false;
    const auto& curr = scopes.back();
    return curr.count(name) > 0;
}

// ----- functions -----
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
