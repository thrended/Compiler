// ============================================================================
//   Author:  Kenneth Perkins
//   Taken From: http://programmingnotes.org/
//   Date:  Jan 29, 2014
//   File: FSM.cpp
//   Description: The following demonstrates the implementation of a simple
//     Lexer using a table driven Finite State Machine.
// ============================================================================
#include <iostream>
#include <cstdlib>
#include <cctype>
#include <string>
#include <vector>
#include <fstream>
using namespace std;
 
/* This holds the transition states for our Finite State Machine
    -- They are placed in numerical order for easy understanding within
	    the FSM array, which is located below */
 
enum FSM_TRANSITIONS
{
    REJECT = 0,
    INTEGER,
    REAL,
    OPERATOR,
    STRING,
    UNKNOWN,
    SPACE
};
 
/* This is the Finite State Machine Table
    -- The zero represents a place holder, so the row in the array starts on
	  row 1 instead of 0.
 
    -- You can tune this table to make the states accept or reject different
	  parameters, thus changing its behavior. More states can be added to
	  this table.
                           integer,  real,  operator,  string,  unknown,  space */
int stateTable[][7] = {{0, INTEGER,  REAL,  OPERATOR,  STRING, UNKNOWN,  SPACE},
/* STATE 1 */   {INTEGER,  INTEGER,  REAL,   REJECT,   REJECT,  REJECT,  REJECT},
/* STATE 2 */   {REAL,       REAL,  UNKNOWN, REJECT,   REJECT,  REJECT,  REJECT},
/* STATE 3 */   {OPERATOR,  REJECT, REJECT,  REJECT,   STRING,  REJECT,  REJECT},
/* STATE 4 */   {STRING,    STRING, REJECT,  STRING,   STRING,  REJECT,  REJECT},
/* STATE 5 */   {UNKNOWN,  UNKNOWN, UNKNOWN, UNKNOWN,  UNKNOWN, UNKNOWN, REJECT},
/* STATE 6 */   {SPACE,     REJECT, REJECT,  REJECT,   REJECT,  REJECT,  REJECT}};
 
 
// struct to hold token information
struct TokenType
{
    string token;
    int lexeme;
    string lexemeName;
};
 
// function prototypes
vector<TokenType> Lexer(string expression);
int Get_FSM_Col(char currentChar);
string GetLexemeName(int lexeme);
 
int main()
{
    // declare variables
    ifstream infile;
    string fileName = "";
    string expression = "";
    vector<TokenType> tokens;
 
    // get data from user
    cout<<"\nPlease enter the name of the file: ";
    getline(cin, fileName);
 
    infile.open(fileName.c_str());
 
    if(infile.fail())
    {
        cout<<"\n** ERROR - the file \""<<fileName<<"\" cannot be found!\n\n";
        exit(1);
    }
 
    // use a loop to scan each line in the file
    while(getline(infile, expression))
    {
        // use the "Lexer" function to isolate integer, real, operator,
        // string, and unknown tokens
        tokens = Lexer(expression);
 
        // display the tokens to the screen
        for(unsigned x = 0; x < tokens.size(); ++x)
        {
            cout<<tokens[x].lexemeName<<"  \t"
                <<tokens[x].token<<endl;
        }
    }
 
    infile.close();
 
    return 0;
}// end of main
 
/**
* FUNCTION: Lexer
* USE: Parses the "expression" string using the Finite State Machine to
*     isolate each individual token and lexeme name in the expression.
* @param expression - A std::string containing text.
* @return - Returns a vector containing the tokens found in the string
*/
vector<TokenType> Lexer(string expression)
{
    TokenType access;
    vector<TokenType> tokens;
    char currentChar = ' ';
    int col = REJECT;
    int currentState = REJECT;
    int prevState = REJECT;
    string currentToken = "";
 
    // use an FSM to parse the expression
    for(unsigned x = 0; x < expression.length();)
    {
        currentChar = expression[x];
 
        // get the column number for the current character
        col = Get_FSM_Col(currentChar);
 
        /* ========================================================
 
            THIS IS WHERE WE CHECK THE FINITE STATE MACHINE TABLE
               USING THE "col" VARIABLE FROM ABOVE ^
 
          ========================================================= */
 
        // get the current state of the expression
        currentState = stateTable[currentState][col];
 
        /* ===================================================
 
          THIS IS WHERE WE CHECK FOR A SUCESSFUL PARSE
           - If the current state of the expression == REJECT
             (the starting state), then we have sucessfully parsed
             a token.
 
            - ELSE we continue trying to find a sucessful token
 
            =================================================== */
        if(currentState == REJECT)
        {
            if(prevState != SPACE) // we dont care about whitespace
            {
                access.token = currentToken;
                access.lexeme = prevState;
                access.lexemeName = GetLexemeName(access.lexeme);
                tokens.push_back(access);
            }
            currentToken = "";
        }
        else
        {
            currentToken += currentChar;
            ++x;
        }
        prevState = currentState;
 
    }
    // this ensures the last token gets saved when
    // we reach the end of the loop (if a valid token exists)
    if(currentState != SPACE && currentToken != "")
    {// ^^ we dont care about whitespace
        access.token = currentToken;
        access.lexeme = currentState;
        access.lexemeName = GetLexemeName(access.lexeme);
        tokens.push_back(access);
    }
    return tokens;
}// end of Lexer
 
/**
* FUNCTION: Get_FSM_Col
* USE: Determines the state of the type of character being examined.
* @param currentChar - A character.
* @return - Returns the state of the type of character being examined.
*/
int Get_FSM_Col(char currentChar)
{
    // check for whitespace
    if(isspace(currentChar))
    {
        return SPACE;
    }
 
    // check for integer numbers
    else if(isdigit(currentChar))
    {
        return INTEGER;
    }
 
    // check for real numbers
    else if(currentChar == '.')
    {
        return REAL;
    }
 
    // check for characters
    else if(isalpha(currentChar))
    {
        return STRING;
    }
 
    // check for operators
    else if(ispunct(currentChar))
    {
        return OPERATOR;
    }
    return UNKNOWN;
}// end of Get_FSM_Col
 
/**
* FUNCTION: GetLexemeName
* USE: Returns the string equivalent of an integer lexeme token type.
* @param lexeme - An integer lexeme token type.
* @return - An std::string string representing the name of the integer
*        lexeme token type.
*/
string GetLexemeName(int lexeme)
{
    switch(lexeme)
    {
        case INTEGER:
           return "INTEGER";
           break;
        case REAL:
           return "REAL  ";
           break;
        case OPERATOR:
           return "OPERATOR";
           break;
        case STRING:
           return "STRING";
           break;
        case UNKNOWN:
           return "UNKNOWN";
           break;
        case SPACE:
           return "SPACE";
           break;
        default:
           return "ERROR";
           break;
    }
}// http://programmingnotes.org/