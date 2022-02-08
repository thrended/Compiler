#include <stdlib.h>
#include <iterator>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>
#include <iomanip>

#include "lexer.cpp"

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

// Acknowledge if this is a statement or not
// DEFAULT: false
bool duringStatement = false;
bool duringExpression = false;

// A vector to hold the tokens
vector<Token> tokens;
// Token token;
int token_ptr;

// Auxiliry functions
void moveup()
{
    if (token_ptr < tokens.size())
    {
        token_ptr += 1;
    }
};

void backdown()
{
    if (token_ptr > 0)
    {
        token_ptr -= 1;
    }
};

// Check if a string is numeric
bool isNumeric(string &s)
{
    for (int i = 0; i < s.size(); i++)
    {
        if (!isdigit(s[i]))
        {
            return false;
        }
    }
    return true;
}

int main()
{
    // Declare variables for reading file
    ifstream infile;
    string fileName = "";
    string line = "";

    // Money sign to mark the end of the file
    // Token moneySign;
    // moneySign.lexemeNum = 4;
    // moneySign.lexeme = '$';
    // moneySign.lexemeName = "SEPERATOR";

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
    while (getline(infile, line))
    {
        vector<Token> temp = lexer(line);
        tokens.insert(tokens.end(), temp.begin(), temp.end());
    }

    // tokens.push_back(moneySign);

    // Parse the tree
    while (token_ptr < tokens.size())
    {
        // Skip comment section
        while (tokens[token_ptr].lexemeNum == 6)
        {
            token_ptr++;
        };

        // Display the token
        cout << left << setw(5) << "Token: " << setw(15) << tokens[token_ptr].lexemeName
             << setw(5) << "Lexeme: " << tokens[token_ptr].lexeme << endl;

        if (duringStatement)
        {
            if (tokens[token_ptr].lexeme == ";")
            {
                duringStatement = false;
                duringExpression = false;
            }
            else if (duringExpression)
            {
                if (checkTerm()) {

                } else if (checkTermPrime()) {
                    checkExpressionPrime();
                };
            }
            else if (!duringExpression)
            {
                if(checkExpression()) {
                    duringExpression = true;
                };
            };
        }
        else if (!duringStatement)
        {
            if (checkStatement())
            {
                duringStatement = true;
            }
        };

        // Move to the next token
        token_ptr++;
        cout << endl;
    }

    // Close the file
    infile.close();
    infile.clear();

    return 0;
}

// Check if the following sentence is <Statement>
bool checkStatement()
{
    bool isStatement = false;
    Token token = tokens[token_ptr];
    // cout << "In checkStatement(), token is " << token.lexeme << endl;

    if (checkAssign())
    {
        cout << "<Statement> -> <Assign>" << endl;
        isStatement = true;
    }
    else if (checkDeclarative())
    {
        cout << "<Statement> -> <Declarative>" << endl;
        isStatement = true;
    }

    return isStatement;
}

// Check if the following sentence is <Assign>
bool checkAssign()
{
    bool isAssign = false;
    Token token = tokens[token_ptr];
    // cout << "In checkAssign(), token is " << token.lexeme << endl;

    if (checkIdentifier())
    {
        moveup();
        token = tokens[token_ptr];

        if (token.lexeme == "=")
        {
            moveup();

            if (checkExpression())
            {
                cout << "<Assign> -> <ID> = <Expression>" << endl;
                isAssign = true;
            }

            backdown();
        }

        backdown();
        token = tokens[token_ptr];
    }

    return isAssign;
}

// Check if the following sentence is <Declarative>
bool checkDeclarative()
{
    bool isDeclarative = false;
    Token token = tokens[token_ptr];
    // cout << "In checkDeclarative(), token is " << token.lexeme << endl;

    if (checkType())
    {
        moveup();

        if (checkIdentifier())
        {
            cout << "<Declarative> -> <Type> <ID>" << endl;
            isDeclarative = true;
        }

        backdown();
    }

    return isDeclarative;
}

// Check if the following sentence is <Expression>
bool checkExpression()
{
    bool isExpression = false;
    Token token = tokens[token_ptr];
    // cout << "In checkExpression(), token is " << token.lexeme << endl;

    if (checkTerm())
    {
        moveup();

        if (checkExpressionPrime())
        {
            cout << "<Expression> -> <Term> <ExpressionPrime>" << endl;
            isExpression = true;
        }

        backdown();
    }

    return isExpression;
}
// Check if the following sentence is <ExpressionPrime>
bool checkExpressionPrime()
{
    bool isExpressionPrime = false;
    Token token = tokens[token_ptr];
    // cout << "In checkExpressionPrime(), token is " << token.lexeme << endl;

    if (token.lexeme == "+" || token.lexeme == "-")
    {
        moveup();

        if (checkTerm())
        {
            moveup();

            if (checkExpressionPrime())
            {
                backdown(); // To display the + or -
                backdown();

                cout << "<ExpressionPrime> -> " << token.lexeme << " <Term> <ExpressionPrime>" << endl;
                isExpressionPrime = true;

                moveup(); // Move back up to avoid backing down too much
                moveup();
            }

            backdown();
        }

        backdown();
    }
    else if (token.lexeme == ")" || token.lexeme == ";")
    {
        cout << "<ExpressionPrime> -> EPSILON" << endl;
        isExpressionPrime = true;
    }

    return isExpressionPrime;
}

// Check if the following sentence is <Term>
bool checkTerm()
{
    bool isTerm = false;
    Token token = tokens[token_ptr];
    // cout << "In checkTerm(), token is " << token.lexeme << endl;

    if (checkFactor())
    {
        moveup();

        if (checkTermPrime())
        {
            cout << "<Term> -> <Factor> <TermPrime>" << endl;
            isTerm = true;
        }

        backdown();
    }

    return isTerm;
}

// Check if the following sentence is <TermPrime>
bool checkTermPrime()
{
    bool isTermPrime = false;
    Token token = tokens[token_ptr];
    // cout << "In checkTermPrime(), token is " << token.lexeme << endl;

    if (token.lexeme == "*" || token.lexeme == "/")
    {
        moveup();

        if (checkFactor())
        {
            moveup();

            if (checkTermPrime())
            {
                backdown(); // To display the + or -
                backdown();

                cout << "<TermPrime> -> " << token.lexeme << " <Factor> <TermPrime>" << endl;
                isTermPrime = true;

                moveup(); // Move back up to avoid backing down too much
                moveup();
            }

            backdown();
        }

        backdown();
    }
    else if (token.lexeme == ")" || token.lexeme == ";" || token.lexeme == "+" || token.lexeme == "-")
    {
        cout << "<TermPrime> -> EPSILON" << endl;
        isTermPrime = true;
    }

    return isTermPrime;
}

// Check if the following sentence is <Factor>
bool checkFactor()
{
    bool isFactor = false;
    Token token = tokens[token_ptr];
    // cout << "In checkFactor(), token is " << token.lexeme << endl;

    if (token.lexeme == "(")
    {
        moveup();

        if (checkExpression())
        {
            moveup();
            token = tokens[token_ptr];

            if (token.lexeme == ")")
            {
                cout << "<Factor> -> ( <Expression> )" << endl;
                isFactor = true;
            };

            backdown();
            token = tokens[token_ptr];
        }

        backdown();
    }
    else if (checkIdentifier())
    {
        cout << "<Factor> -> <ID>" << endl;
        isFactor = true;
    }
    else if (isNumeric(token.lexeme))
    {
        cout << "<Factor> -> num" << endl;
        isFactor = true;
    }

    return isFactor;
}

// Check if the following sentence is <Identifier>
bool checkIdentifier()
{
    bool isIdentifier = false;
    Token token = tokens[token_ptr];
    // cout << "In checkIdentifier(), token is " << token.lexeme << endl;

    if (token.lexemeNum == IDENTIFIER)
    {
        cout << "<ID> -> " << token.lexeme << endl;
        isIdentifier = true;
    }

    return isIdentifier;
}

// Check if the following sentence is <Type>
bool checkType()
{
    bool isType = false;
    Token token = tokens[token_ptr];
    // cout << "In checkType(), token is " << token.lexeme << endl;

    if (token.lexemeNum == KEYWORD)
    {

        if (token.lexeme == "bool" || token.lexeme == "float" || token.lexeme == "int")
        {
            cout << "<Type> -> " << token.lexeme << endl;
            isType = true;
        }
    }

    return isType;
}