#include "scanner.h"
#include "parser.h"
#include <iostream>
using namespace std;

int main() {
    string code = R"(
        #oil <speed>
        key racetrack

        ignite() {
            gear fuel = 20;
            track (X>5<3) {
                announce "Low fuel!";
            }
            pitstop {
                announce "Refueling...";
            
            finishline 0;
            }
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
