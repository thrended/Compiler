/**********

Lexical Analyzer

Group Members: Duc Nguyen (887323301), Wayne Lin (887280121)

**********/

/**************
Instructions
1) Run the .exe
2) Enter name of text file when prompted
3) View results
**************/

#include <stdlib.h>
#include <iterator>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

/*
    Enumerator to get lexeme name and to transist the FSM
*/
enum TOKEN_TYPES
{
	REJECT = 0,
	INTEGER,
	REAL,
	SEPERATOR,
	OPERATOR,
	STRING,
	IGNORE,
	UNKNOWN,
	SPACE
};

// State table for transvering through the lexeme
// A string can either be a KEYWORD or an IDENTIFIER
int stateTable[][9] = {
	/* PLACE HOLDER STATE */ {0, INTEGER, REAL, SEPERATOR, OPERATOR, STRING, IGNORE, UNKNOWN, SPACE},
	/* INTEGER STATE */ {INTEGER, INTEGER, REAL, REJECT, REJECT, REJECT, REJECT, REJECT, REJECT},
	/* REAL STATE */ {REAL, REAL, UNKNOWN, REJECT, REJECT, REJECT, REJECT, REJECT, REJECT},
	/* SEPERATOR STATE */ {SEPERATOR, REJECT, REJECT, REJECT, REJECT, REJECT, REJECT, REJECT, REJECT},
	/* OPERATOR STATE */ {OPERATOR, REJECT, REJECT, REJECT, OPERATOR, STRING, REJECT, REJECT, REJECT},
	/* STRING STATE */ {STRING, STRING, STRING, REJECT, STRING, STRING, STRING, STRING, REJECT},
	/* IGNORE STATE */ {IGNORE, IGNORE, IGNORE, IGNORE, IGNORE, IGNORE, REJECT, IGNORE, IGNORE},
	/* UNKNOWN STATE */ {UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN},
	/* REJECT STATE */ {SPACE, REJECT, REJECT, REJECT, REJECT, REJECT, REJECT, REJECT, REJECT}};

/*
    A struct to hold a token information
*/
struct Token
{
	int lexemeNum;
	string token;
	string lexemeName;
};

// Set to hold seperators
static const unordered_set<char> SEPERATORS({'{', '}', '[', ']', '(', ')', ',', '.', ';', ':'});
// Set to hold operators
static const unordered_set<char> OPERATORS({'+', '-', '*', '/', '=', '<', '>', '%'});
// Set to hold special characters
static const unordered_set<char> SPECIALS({'!', '_'});
// Set to hold keywords
static const unordered_set<string> KEYWORDS({"int", "float", "bool", "True", "False",
											 "if", "else", "then", "endif", "endelse", "while", "whileend", "do",
											 "enddo", "for", "endfor", "STDinput", "STDoutput", "and", "or", "not"});

/* PROTOTYPES FOR THE FUNCTIONS */
vector<Token> lexer(string expression);
int getCharState(char currentChar, char prevState);
string getLexemeName(int lexemeNum, string token);

int main()
{
	// Declare variables for reading file
	ifstream infile;
	string fileName = "";
	string line = "";

	// A vector hold the tokens
	vector<Token> tokens;

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

  cout << "TOKENS                  LEXEMES" << endl << endl;

	// Get each line in the text file
	while (getline(infile, line))
	{
		tokens = lexer(line);

		// Display the tokens to the screen
		for (unsigned x = 0; x < tokens.size(); ++x)
		{
			// Don't print out the comment
			if (tokens[x].lexemeNum != IGNORE)
			{
			cout << tokens[x].lexemeName << "  \t"
				 << tokens[x].token << endl;
			}
		}
	}

	// Close the file
	infile.close();
	infile.clear();

	return 0;
}

/**
*	FUNCTION: lexer
*	USE: go through the string to return a list of tokens.
*	@param expression - the code line
*	@return tokens - a list of tokens
*/
vector<Token> lexer(string expression)
{
	// Variable for the state machine
	Token access;
	vector<Token> tokens;
	char currentChar = ' ';
	int charState = REJECT;
	int currentState = REJECT;
	int prevState = REJECT;
	string currentToken = "";

	// Go through each character
	for (unsigned x = 0; x < expression.length();)
	{

		// Check the current state of the character
		currentChar = expression[x];
		charState = getCharState(currentChar, prevState);

		// Get the current state
		currentState = stateTable[currentState][charState];

		// If the current state is REJECT, we have started a new parse,
		// and the old one is succesfull.
		// If not, continue finding a token
		if (currentState == REJECT)
		{
			if (prevState != SPACE) // we dont care about whitespace
			{
				access.token = currentToken;
				access.lexemeNum = prevState;
				access.lexemeName = getLexemeName(access.lexemeNum, access.token);
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

	// Include the last character
	if (currentState != SPACE && currentToken != "")
	{
		access.token = currentToken;
		access.lexemeNum = currentState;
		access.lexemeName = getLexemeName(access.lexemeNum, access.token);
		tokens.push_back(access);
	}
	return tokens;
} // end of Lexer

/*
* FUNCTION: getCharState
* USE: get the corresponding state based on the current character
* @param currentChar - the current char
* @return int - the current state of the character
*/
int getCharState(char currentChar, char prevState)
{
	// Check for whitespace
	if (isspace(currentChar))
	{
		return SPACE;
	}

	// Check for integer numbers
	else if (isdigit(currentChar))
	{
		return INTEGER;
	}

	// Check for real numbers or period
	else if (currentChar == '.')
	{
		return (prevState == INTEGER ? REAL : SEPERATOR);
	}

	// Check for characters
	else if (isalpha(currentChar))
	{
		return STRING;
	}

	// Check for comment
	else if (currentChar == '!')
	{
		return IGNORE;
	}

	// Check for seperators
	else if (SEPERATORS.find(currentChar) != SEPERATORS.end())
	{
		return SEPERATOR;
	}

	// Check for operators
	else if (OPERATORS.find(currentChar) != OPERATORS.end())
	{
		return OPERATOR;
	}

	return UNKNOWN;
}

/*
* FUNCTION: getLexemeName
* USE: takes a number and return the corresponding string to the token type.
* @param lexemeNum - integer that corresponds to the state
* @return - a string of the state
*/
string getLexemeName(int lexemeNum, string token)
{
	switch (lexemeNum)
	{
	case INTEGER:
		return "INTEGER         =    ";
		break;
	case REAL:
		return "REAL            =    ";
		break;
	case SEPERATOR:
		return "SEPERATOR       =    ";
		break;
	case OPERATOR:
		return "OPERATOR        =   ";
		break;
	case STRING:
		return (KEYWORDS.find(token) != KEYWORDS.end() ? "KEYWORD         =    " : "IDENTIFER       =    ");
		break;
	case IGNORE:
		return "COMMENT         =   ";
		break;
	case UNKNOWN:
		return "UNKNOWN         =   ";
		break;
	case SPACE:
		return "SPACE           =   ";
		break;
	default:
		return "ERROR           =   ";
		break;
	};
};
