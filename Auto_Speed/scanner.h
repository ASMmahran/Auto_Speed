#pragma once

#include <string>
#include <vector>
using namespace std;

enum TokenType {
    KEYWORD,
    IDENTIFIER,
    NUMBER,
    STRING,
    OPERATOR,
    SYMBOL,
    BOOLEAN,
    END_OF_FILE,
    UNKNOWN // Added for any character that doesn't match
};


struct Token {
    TokenType type;
    string value;
    int line; // The line number where the token was found
};

/*
 * scan
 * The main scanner function.
 * Takes raw code as a string and returns a vector of Tokens.
 */
vector<Token> scan(const string& code);


/*
 * tokenTypeToString
 * A helper function to get a printable name for a TokenType.
 */
string tokenTypeToString(TokenType type);

