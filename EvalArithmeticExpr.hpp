#include <algorithm>
#include <cmath>
#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <string_view>

using namespace std;

struct Node {
    char value{};
    Node *left{};
    Node *right{};
};

inline void prettyPrintTree(const string &prefix, const Node *root, const bool isLeft) {
    if (!root) {
        return;
    }

    cout
            << prefix
            << (isLeft ? "├──" : "└──")
            << root->value
            << endl;

    int count{};
    if (root->left) ++count;
    if (root->right) ++count;

    prettyPrintTree(prefix + (isLeft ? "│   " : "    "), root->left, count % 2 == 0);
    prettyPrintTree(prefix + (isLeft ? "│   " : "    "), root->right, false);
}

inline void prettyPrintTree(const Node *root) {
    prettyPrintTree("", root, false);
}

template<typename T>
void printQueue(queue<T> input) {
    while (!input.empty()) {
        cout << ' ' << input.front();
        input.pop();
    }

    cout << ".\n";
}

inline void removeSpaces(string &input) {
    input.erase(remove_if(input.begin(), input.end(), [](unsigned char c) { return isspace(c); }), input.end());
}

inline string_view getAssociativity(const char input) {
    switch (input) {
        case '-':
        case '+':
        case '/':
        case '*':
            return "left";

        case '^':
            return "right";

        default:
            cerr << "getAssociativity(): unknown operator.\n";
            exit(EXIT_FAILURE);
    }
}

inline int getPrecedence(const char input) {
    switch (input) {
        case '-':
        case '+':
            return 1;

        case '/':
        case '*':
            return 2;

        case '^':
            return 3;

        default:
            cerr << "getPrecedence(): unknown operator.\n";
            exit(EXIT_FAILURE);
    }
}

inline queue<char> theShuntingYard(const string_view input) {
    queue<char> output{};
    stack<char> symbols{};

    for (char c: input) {
        if (isdigit(c)) {
            output.push(c);
        } else if (c == '(') {
            symbols.push(c);
        } else if (c == ')') {
            while (!symbols.empty() && symbols.top() != '(') {
                output.push(symbols.top());
                symbols.pop();
            }

            symbols.pop();
        } else {
            while (
                (!symbols.empty() && symbols.top() != '(') &&
                (getPrecedence(symbols.top()) > getPrecedence(c) || (
                     (getPrecedence(symbols.top()) == getPrecedence(c)) && (getAssociativity(c) == "left")))
            ) {
                output.push(symbols.top());
                symbols.pop();
            }

            symbols.push(c);
        }
    }

    while (!symbols.empty()) {
        output.push(symbols.top());
        symbols.pop();
    }

    return output;
}

inline Node *createExpressionTree(queue<char> postfix) {
    stack<Node *> result{};

    while (!postfix.empty()) {
        char c{postfix.front()};
        postfix.pop();

        if (isdigit(c)) {
            Node *digit{new Node};
            digit->value = c;

            result.push(digit);
        } else {
            Node *operation{new Node};
            operation->value = c;

            operation->right = result.top();
            result.pop();

            operation->left = result.top();
            result.pop();

            result.push(operation);
        }
    }

    return result.top();
}

inline double evaluateTree(const Node *root) {
    if (!root) {
        cerr << "evaluateTree(): invalid tree.\n";
        exit(EXIT_FAILURE);
    }

    if (isdigit(root->value)) {
        return static_cast<double>(root->value - '0');
    }

    switch (root->value) {
        case '-': return evaluateTree(root->left) - evaluateTree(root->right);
        case '+': return evaluateTree(root->left) + evaluateTree(root->right);
        case '/': return evaluateTree(root->left) / evaluateTree(root->right);
        case '*': return evaluateTree(root->left) * evaluateTree(root->right);
        case '^': return pow(evaluateTree(root->left), evaluateTree(root->right));

        default:
            cerr << "evaluateTree(): unknown operator.\n";
            exit(EXIT_FAILURE);
    }
}