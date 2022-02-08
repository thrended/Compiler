#include <stdlib.h>
#include <iterator>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>
#include <iomanip>
#include <cstdio>
#include <ctime>

#include "lexer_v101.cpp"
#include "expr.cpp"

using namespace std;

bool checkStatement();
bool checkAssign();
bool checkDeclarative();

bool checkExpression();
bool checkExpressionPrime();
bool checkTerm();
bool checkTermPrime();
bool checkFactor();

bool checkIdentifier();
bool checkType();

clock_t t;
double duration;

// Acknowledge if this is a statement or not
// DEFAULT: false
bool duringStatement = false;
bool duringExpression = false;

// A vector to hold the tokens
vector<Token> tokens;
// Token token;
int token_ptr;

int main()
{
    // Declare variables for reading file
    ifstream infile;
    string fileName = "";
    string line = "";

    // Money sign to mark the end of the file
    Token moneySign;
    moneySign.lexemeNum = 4;
    moneySign.lexeme = "$";
    moneySign.lexemeName = "SEPERATOR";
    // Fake semicolon
    Token semicolon;
    semicolon.lexemeNum = 3;
    semicolon.lexeme = ";";
    semicolon.lexemeName = "SEPERATOR";
    semicolon.artificial = true;

    // Read the file
    cout << "\nPlease enter the name of the file: ";
    getline(cin, fileName);

    // Open the file
    infile.open(fileName.c_str());

    // Error shooting if the file cannot be opened
    if (infile.fail())
    {
        cout << "\n** ERROR - the file \"" << fileName << "\" cannot be found!\n\n";
        exit(1);
    }

    token_ptr = 0;

    // Determine each token in the line
    // Forces a semicolon at the end of lines to ensure a boundary exists between sentences
    while (getline(infile, line))
    {
        vector<Token> temp = lexer(line);
        if (!temp.empty() && temp.back().lexeme != ";")
        {            
            temp.push_back(semicolon);
        }
        tokens.insert(tokens.end(), temp.begin(), temp.end());
    }

    tokens.push_back(moneySign);

    // Close the file
    infile.close();
    infile.clear();
    /*
    // Parse the file one sentence (line) at a time
    // Expr is a helper class to determine syntax using tabular method
    */
    Expr expr;
    string tempstring, prev;
    t = clock();

    for (Token tk : tokens)
    {
        tempstring = tk.lexeme;
        expr.str2 += tempstring;
        expr.token = tk;
        prev = tk.lexeme;
        if (tk.lexeme == ";")
        {
            if (tk.artificial)
            {
                expr.setArtificial(true);
            }
            expr.interpret();
            expr.str2 = "";
            expr.setArtificial(false);
        }
        expr.prev = tempstring;

    }
    duration = ( clock() - t ) / (double) CLOCKS_PER_SEC;
    cout << "\nParsing concluded successfully in " << duration << " seconds! \n\n";

    return 0;
}
