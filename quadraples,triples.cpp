#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <algorithm> // for std::remove

using namespace std;

struct Quadruple {
    string op, arg1, arg2, result;
};

struct Triple {
    string op, arg1, arg2;
};

void generateIntermediateCode(const string& input) {
    string expr = input;
    vector<Quadruple> quads;
    vector<Triple> triples;

    string lhs, rhs;
    size_t eq = expr.find('=');

    if (eq == string::npos) {
        cout << "Invalid expression.\n";
        return;
    }

    lhs = expr.substr(0, eq);
    rhs = expr.substr(eq + 1);

    // Remove spaces
    lhs.erase(std::remove(lhs.begin(), lhs.end(), ' '), lhs.end());
    rhs.erase(std::remove(rhs.begin(), rhs.end(), ' '), rhs.end());

    // Tokenize the right-hand side expression
    vector<string> tokens;
    for (size_t i = 0; i < rhs.size(); ++i) {
        if (isalnum(rhs[i])) {
            tokens.push_back(string(1, rhs[i]));
        } else if (rhs[i] == '+' || rhs[i] == '-' || rhs[i] == '*' || rhs[i] == '/') {
            tokens.push_back(string(1, rhs[i]));
        }
    }

    int tempCount = 1;

    // Handle * and /
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i] == "*" || tokens[i] == "/") {
            string temp = "t" + to_string(tempCount++);
            quads.push_back({tokens[i], tokens[i - 1], tokens[i + 1], temp});
            triples.push_back({tokens[i], tokens[i - 1], tokens[i + 1]});
            tokens[i - 1] = temp;
            tokens.erase(tokens.begin() + i, tokens.begin() + i + 2);
            i = 0;
        }
    }

    // Handle + and -
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i] == "+" || tokens[i] == "-") {
            string temp = "t" + to_string(tempCount++);
            quads.push_back({tokens[i], tokens[i - 1], tokens[i + 1], temp});
            triples.push_back({tokens[i], tokens[i - 1], tokens[i + 1]});
            tokens[i - 1] = temp;
            tokens.erase(tokens.begin() + i, tokens.begin() + i + 2);
            i = 0;
        }
    }

    // Final assignment
    quads.push_back({"=", tokens[0], "-", lhs});
    triples.push_back({"=", tokens[0], "-"});

    // Print Quadruples
    cout << "\nQuadruples:\n";
    cout << "Op\tArg1\tArg2\tResult\n";
    for (auto& q : quads)
        cout << q.op << "\t" << q.arg1 << "\t" << q.arg2 << "\t" << q.result << "\n";

    // Print Triples
    cout << "\nTriples:\n";
    cout << "Index\tOp\tArg1\tArg2\n";
    for (size_t i = 0; i < triples.size(); ++i)
        cout << i << "\t" << triples[i].op << "\t" << triples[i].arg1 << "\t" << triples[i].arg2 << "\n";

    // Print Indirect Triples
    cout << "\nIndirect Triples:\n";
    cout << "Pointer\t=> Triple Index\n";
    for (size_t i = 0; i < triples.size(); ++i)
        cout << "P" << i << "\t=> " << i << "\n";
}

int main() {
    string input;
    cout << "Enter expression (e.g., a = b + c * d): ";
    getline(cin, input);
    generateIntermediateCode(input);
    return 0;
}