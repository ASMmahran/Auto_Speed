#include "scanner.h" 
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <cctype>
using namespace std;



unordered_set<string> keywords = {
    "ignite", "engine",
    "gear", "turbo", "exhaust", "flag",
    "announce", "listen",
    "track", "pitstop", "looplap", "overtake", "finishline",
    "key", "#oil", "#car"
};

unordered_set<string> booleans = { "true", "false" };
unordered_set<char> operators = { '+', '-', '*', '/', '=', '<', '>' };
unordered_set<char> symbols = { '{', '}', '(', ')', ';' };
// ---


vector<Token> scan(const string& code) {
    vector<Token> tok;
    int i = 0;
    int line = 1; 

    while (i < code.size()) {
        // 1. Skip Whitespace
        if (isspace(code[i])) {
            if (code[i] == '\n') line++; // Increment line counter
            i++;
            continue;
        }

        // 2. String
        if (code[i] == '"') {
            string str;
            i++; // Consume opening "
            int startLine = line; // In case string is multi-line

            while (i < code.size() && code[i] != '"') {
                if (code[i] == '\n') line++; // Support multi-line strings
                str += code[i++];
            }

            if (i == code.size()) {
                cout << "Error [Line " << startLine << "]: Unterminated string!" << endl;
                break; // Stop scanning
            }
            else {
                tok.push_back({ STRING, str, startLine });
                i++; // Consume closing "
            }
        }
        // 3. Identifier or Keyword
        else if (isalpha(code[i]) || code[i] == '#') {
            string word;
            while (i < code.size() && (isalnum(code[i]) || code[i] == '#' || code[i] == '_')) {
                word += code[i++];
            }

            if (keywords.count(word))
                tok.push_back({ KEYWORD, word, line });
            else if (booleans.count(word))
                tok.push_back({ BOOLEAN, word, line });
            else
                tok.push_back({ IDENTIFIER, word, line });
        }
        // 4. Number
        else if (isdigit(code[i])) {
            string num;
            bool hasDecimal = false;
            while (i < code.size() && (isdigit(code[i]) || code[i] == '.')) {
                if (code[i] == '.') {
                    if (hasDecimal) break; // Found a second decimal
                    hasDecimal = true;
                }
                num += code[i++];
            }
            tok.push_back({ NUMBER, num, line });
        }
        // 5. Operator
        else if (code[i] == '+' || code[i] == '-' || code[i] == '*' || code[i] == '/' || code[i] == '=' || code[i] == '<' || code[i] == '>') {

            // Check for two-char operators
            if (i + 1 < code.length()) {
                char n = code[i + 1];

                if ((code[i] == '=' && n == '=') ||
                    (code[i] == '<' && n == '=') ||
                    (code[i] == '>' && n == '=') ||
                    (code[i] == '!' && n == '=')) {

                    string op;
                    op += code[i];
                    op += n;
                    tok.push_back({ OPERATOR, op, line });
                    i += 2;
                    continue;
                }
            }

            // Single char operator
            tok.push_back({ OPERATOR, string(1, code[i]), line });
            i++;
            continue;
        }


        // 6. Symbol
        else if (symbols.count(code[i])) {
            string sym(1, code[i++]);
            tok.push_back({ SYMBOL, sym, line });
        }
        // 7. Unknown
        else {
            string unknown(1, code[i++]);
            cout << "Error [Line " << line << "]: Unknown character: " << unknown << endl;
            tok.push_back({ UNKNOWN, unknown, line });
        }
    }
    tok.push_back({ END_OF_FILE, "EOF", line });
    return tok;
}


/*
 * Implementation of the helper function
 */
string tokenTypeToString(TokenType type) {
    switch (type) {
    case KEYWORD:     return "KEYWORD";
    case IDENTIFIER:  return "IDENTIFIER";
    case NUMBER:      return "NUMBER";
    case STRING:      return "STRING";
    case OPERATOR:    return "OPERATOR";
    case SYMBOL:      return "SYMBOL";
    case BOOLEAN:     return "BOOLEAN";
    case END_OF_FILE: return "END_OF_FILE";
    case UNKNOWN:     return "UNKNOWN";
    default:          return "ERROR";
    }
}

