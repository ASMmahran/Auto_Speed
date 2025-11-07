#include "parser.h"
#include <iostream>
#include <stdexcept>
using namespace std;

// ============= Utility functions =============
Parser::Parser(const vector<Token>& t) : tokens(t), pos(0) {}

Token Parser::current() {
    if (pos >= tokens.size()) return { END_OF_FILE, "EOF", -1 };
    return tokens[pos];
}

void Parser::advance() { if (pos < tokens.size()) pos++; }

bool Parser::match(TokenType t) {
    if (current().type == t) { advance(); return true; }
    return false;
}

void Parser::expect(TokenType t, const string& msg) {
    if (!match(t)) error("Expected " + msg + ", found '" + current().value + "'");
}

void Parser::error(const string& msg) {
    hasError = true;
    cerr << "❌ Parse Error [Line " << current().line << "]: " << msg << endl;
    // Attempt simple recovery: skip current token
    advance();
}

void Parser::printNode(const string& label) {
    cout << string(indentLevel * 2, ' ') << label << endl;
}

// ============= Grammar rules =============

void Parser::parse() {
    cout << " Starting Auto-Speed parse...\n";
    Program();
    if (!hasError)
        cout << " Parsing completed successfully!\n";
    else
        cout << " Parsing completed with errors.\n";
}

void Parser::Program() {
    printNode("Program");
    indentLevel++;
    ImportSection();
    NamespaceSection();
    FunctionList();
    indentLevel--;
}

void Parser::ImportSection() {
    printNode("ImportSection");
    indentLevel++;
    while (current().value == "#oil") {
        printNode("Import: " + current().value);
        advance();
        if (current().value == "<") advance();
        if (current().type == IDENTIFIER) advance();
        if (current().value == ">") advance();
    }
    indentLevel--;
}

void Parser::NamespaceSection() {
    printNode("NamespaceSection");
    indentLevel++;
    if (current().value == "key") {
        advance();
        if (current().type == IDENTIFIER) {
            printNode("Namespace: " + current().value);
            advance();
        }
        else error("Expected namespace name after 'key'");
    }
    else error("Expected 'key' keyword");
    indentLevel--;
}

void Parser::FunctionList() {
    printNode("FunctionList");
    indentLevel++;
    while (current().value == "engine" || current().value == "ignite") {
        Function();
    }
    indentLevel--;
}

void Parser::Function() {
    printNode("Function");
    indentLevel++;
    string fname = current().value;
    advance(); // engine or ignite
    if (fname == "engine" && current().type == IDENTIFIER)
        advance();
    expect(SYMBOL, "(");
    expect(SYMBOL, ")");
    Block();
    indentLevel--;
}

void Parser::Block() {
    printNode("Block");
    indentLevel++;
    expect(SYMBOL, "{");
    StatementList();
    expect(SYMBOL, "}");
    indentLevel--;
}

void Parser::StatementList() {
    printNode("StatementList");
    indentLevel++;
    while (current().value != "}" && current().type != END_OF_FILE) {
        Statement();
    }
    indentLevel--;
}

void Parser::Statement() {
    printNode("Statement");
    indentLevel++;
    string val = current().value;
    if (val == "announce") OutputStmt();
    else if (val == "listen") InputStmt();
    else if (val == "track") ConditionalStmt();
    else if (val == "looplap") LoopStmt();
    else if (val == "finishline") ReturnStmt();
    else if (current().type == KEYWORD) DeclarationStmt();
    else if (current().type == IDENTIFIER) AssignmentStmt();
    else advance();
    indentLevel--;
}

void Parser::DeclarationStmt() {
    printNode("DeclarationStmt");
    indentLevel++;
    advance(); // type
    expect(IDENTIFIER, "variable name");
    expect(OPERATOR, "=");
    Expression();
    expect(SYMBOL, ";");
    indentLevel--;
}

void Parser::AssignmentStmt() {
    printNode("AssignmentStmt");
    indentLevel++;
    advance(); // identifier
    expect(OPERATOR, "=");
    Expression();
    expect(SYMBOL, ";");
    indentLevel--;
}

void Parser::ConditionalStmt() {
    printNode("ConditionalStmt");
    indentLevel++;
    advance(); // track
    expect(SYMBOL, "(");
    Expression();
    expect(SYMBOL, ")");
    Block();
    if (current().value == "pitstop") {
        advance();
        Block();
    }
    indentLevel--;
}

void Parser::LoopStmt() {
    printNode("LoopStmt");
    indentLevel++;
    advance();
    expect(SYMBOL, "(");
    Expression();
    expect(SYMBOL, ")");
    Block();
    indentLevel--;
}

void Parser::OutputStmt() {
    printNode("OutputStmt");
    indentLevel++;
    advance();
    Expression();
    expect(SYMBOL, ";");
    indentLevel--;
}

void Parser::InputStmt() {
    printNode("InputStmt");
    indentLevel++;
    advance();
    expect(IDENTIFIER, "variable name");
    expect(SYMBOL, ";");
    indentLevel--;
}

void Parser::ReturnStmt() {
    printNode("ReturnStmt");
    indentLevel++;
    advance();
    Expression();
    expect(SYMBOL, ";");
    indentLevel--;
}

void Parser::Expression() {
    printNode("Expression");
    indentLevel++;
    Term();
    while (current().type == OPERATOR &&
        (current().value == "+" || current().value == "-" ||
            current().value == "<" || current().value == ">" ||
            current().value == "==" || current().value == "!=")) {
        advance();
        Term();
    }
    indentLevel--;
}

void Parser::Term() {
    printNode("Term");
    indentLevel++;
    Factor();
    while (current().type == OPERATOR &&
        (current().value == "*" || current().value == "/" || current().value == "%")) {
        advance();
        Factor();
    }
    indentLevel--;
}

void Parser::Factor() {
    printNode("Factor");
    indentLevel++;
    if (current().type == NUMBER || current().type == STRING || current().type == IDENTIFIER) {
        advance();
    }
    else if (current().value == "(") {
        advance();
        Expression();
        expect(SYMBOL, ")");
    }
    else {
        error("Expected valid expression");
        advance();
    }
    indentLevel--;
}
