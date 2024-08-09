#include <iostream>
#include <string>

#include "EvalArithmeticExpr.hpp"

using namespace std;

int main() {
    string infix{};
    cout << "Type an arithmetic expression: ";

    getline(cin >> ws, infix);
    removeSpaces(infix);

    queue<char> postfix{theShuntingYard(infix)};
    cout << "The postfix notation is:";
    printQueue(postfix);

    Node *tree{createExpressionTree(postfix)};
    cout << "The corresponding expression tree is:\n";
    prettyPrintTree(tree);

    cout << endl;
    cout << "The value of the expression is: " << evaluateTree(tree) << ".\n";

    return EXIT_SUCCESS;
}