#include "scanner.h"
#include "parser.h"
#include <iostream>
using namespace std;

int main() {
    string code = R"(
ignite() {
    {
        gear x = 5;
        announce x;
    }
    announce x;
    finishline 0;
}

    )";

    vector<Token> tokens = scan(code);

    cout << "=== TOKENS ===\n";
    for (auto& t : tokens)
        cout << tokenTypeToString(t.type) << " : " << t.value << " (line " << t.line << ")\n";

    cout << "\n=== PARSER ===\n";
    Parser p(tokens);
    p.parse();
}
