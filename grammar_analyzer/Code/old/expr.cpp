#include <stdlib.h>
#include <iterator>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <set>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <stack>

//#include "lexer_v101.cpp"

using namespace std;

/*
    Enumerator (currently unused)
*/

//#ifndef _MY_HEADER_H_
//#define _MY_HEADER_H_


//unused for now
enum EXPR_TYPES
{
	INVALID = -1,
	Te,
	PLUSTe,
	MINUSTe,
	Ft,
	STARFt,
	SLASHFt,
	id,
	EPSILON,
	E_WITH_PARENTHESES,
    STAT,
    ASSIGN,
	NOTHNG
};

//#endif /* _MY_HEADER_H_ */
// Set to hold seperators
static const unordered_set<char> SEPS({'{', '}', '[', ']', '(', ')', ',', '.', ';', ':'});
// Set to hold operators
static const unordered_set<char> OPS({'+', '-', '*', '/', '=', '<', '>', '%'});
// Set to hold special characters
static const unordered_set<char> SPEC({'!', '_'});
// Set to hold keywords
static const unordered_set<string> KEYS({"int", "float", "bool", "True", "False",
											 "if", "else", "then", "endif", "endelse", "while", "whileend", "do",
											 "enddo", "for", "endfor", "STDinput", "STDoutput", "and", "or", "not"});

class Expr 
{
    public:
    string teststring[6] = {"i","+","i","*","i","$"};
    vector<string> teststringvec;
    stack<string> stk;
    string str2;
    vector<string> expanded;
    unordered_set<string> expansion;
    string str3;
    int curr = 0;
    bool assigned = false;
    bool once = true;
    string op;

    bool isTerminal(string S) 
    {
        return  S == "*" || S == "/" || 
                S == "+" || S == "-" ||
                S == "i" || S == "#" || 
                S == "(" || S == ")";

    }

    string RULES[5][10] = {
    //        id      +     -      *         /      (       )       $       ;         =
            {"Te",  "nil", "nil",  "nil",  "nil",  "Te",   "nil",  "nil",   "",    "skip"},    // E        E  ->  T E'                  in the table : e = E'
            {"nil", "+Te", "-Te",  "nil",  "nil",  "nil",   "",    "",      "",    "skip"},    // E’       E' ->  +- T E' | epsilon     in the table : e = E' , "" = epsilon
            {"Ft",  "nil", "nil",  "nil",  "nil",  "Ft",   "nil",  "nil",   "",    "skip"},    // T        T  ->  F T'                  in the table : t = T'
            {"nil",   "",    "",   "*Ft",  "/Ft",  "nil",   "",     "",     "",    "skip"},    // T’       T' ->  */ F T' | epsilon     in the table : t = T' , "" = epsilon
            {"i",   "nil", "nil",  "nil",  "nil",  "(E)",  "nil",  "nil",  "nil",  "skip"}     // F        F  ->  ( E )   | id          in the table : i stands for id
    };

    int newRULES[5][10] = {
    //        id          +          -          *         /             (                   )          $          ;         =
            {Te,       INVALID,   INVALID,   INVALID,   INVALID,     Te,                 INVALID,   NOTHNG,   EPSILON,   NOTHNG},    // E
            {INVALID,  PLUSTe,    MINUSTe,   INVALID,   INVALID,     INVALID,            EPSILON,   NOTHNG,   EPSILON,   NOTHNG},    // E’
            {Ft,       INVALID,   INVALID,   INVALID,   INVALID,     Ft,                 INVALID,   NOTHNG,   EPSILON,   NOTHNG},    // T
            {INVALID,  EPSILON,   EPSILON,   STARFt,    SLASHFt,     INVALID,            EPSILON,   NOTHNG,   EPSILON,   NOTHNG},    // T’
            {id,       INVALID,   INVALID,   INVALID,   INVALID,     E_WITH_PARENTHESES, INVALID,   NOTHNG,   EPSILON,   NOTHNG}     // F
    };


    string table(string X, string a)
    {
        int aIdx, XIdx=-1;
        if (isalnum(a[0]))
        {
            a = "i";
        }
        switch (a[0]) 
        {
            case 'i' : aIdx=0; break;
            case '+' : aIdx=1; break;
            case '-' : aIdx=2; break;
            case '*' : aIdx=3; break;
            case '/' : aIdx=4; break;
            case '(' : aIdx=5; break;
            case ')' : aIdx=6; break;
            case '$' : aIdx=7; break;
            case ';' : aIdx=8; break;
            case '=' : aIdx=9; break;
            case '#' : aIdx=9; break;
            default: cout << "ERROR while looking up table\n"; break;
        }
        switch (X[0]) 
        {
            case 'E' : XIdx=0; break;
            case 'e' : XIdx=1; break;
            case 'T' : XIdx=2; break;
            case 't' : XIdx=3; break;
            case 'F' : XIdx=4; break;
        }
        return RULES[XIdx][aIdx];
    }

    string convert(string S)
    {
        if (S == "") {return NULL;}
        string tmp;
        if (!assigned)
        {
            if (once)
            {
                tmp = "     <Statement> -> <Assign>\n     <Assign> -> <ID> = <Expression>;\n";
                cout << tmp;
                once = false;
            }
            
            return "";
        }
        for (char c : S)
        {
            switch(c)
            {
                case 'E' : 
                    tmp += "    <Expression> -> <Term> <ExpressionPrime> ";
                    break;
                case 'e' :
                    tmp += "    <Expression Prime> -> "+op+" <Term> <ExpressionPrime> | <Empty> ";
                    break;
                case 'T' :
                    tmp += "    <Term> -> <Factor> <TermPrime> ";
                    break;
                case 't' :
                    tmp += "    <Term Prime> -> "+op+" <Factor> <TermPrime> | <Empty> ";
                    break;
                case 'F' :
                    tmp += "    <Factor> -> ( Expression ) | <ID> | <Num> ";
                    break;
                case 'i' :
                    tmp += "    <ID> -> "+str2.substr(curr,1);
                    break;
                case '~' :
                    tmp += "    <Empty> -> Epsilon ";
                    break;
                case '+' :  
                    op = '+';
                    break;
                case '-' :
                    op = '-'; 
                    break;
                case '*' :
                    op = '*';
                    break;       
                case '/' :
                    op = '/';
                    break;
                
            }
        }
        //cout << "Converted string for EXPRRULE " << S << " is " << tmp << endl;
        if (tmp!= "") { cout << tmp << endl; }
        return tmp;
    }

    string getLex(string tk)
    {
        if (isalpha(tk[0]))
        {
            return (KEYS.find(tk) != KEYS.end() ? "KEYWORD" : "IDENTIFIER");
        }
        if (isdigit(tk[0]))
        {
            return "INTEGER";
        }
        if (OPS.find(tk[0]) != OPS.end())
        {
            return "OPERATOR";
        }
        if (SEPS.find(tk[0]) != SEPS.end())
        {
            return "SEPARATOR";
        }
        return NULL;
    }

    bool interpret(vector<string> input, int current = 0)
    {

        stk.push("$"); stk.push("E");
        string a, X;
        for(;;) {
            cout << "Token = " << input[current] << endl;
            cout << "STACK TOP VALUE = " << stk.top() << endl;
            a = input[current];
            X = stk.top();
            if (isTerminal(X) || X == "$") 
            {
                if (a == X) 
                {
                    stk.pop(); 
                    current++;
                }   
                else {return false;}
            } 
            else
            {
                stk.pop();
                string prod = table(X,a);
                if (prod=="nil") {return false;}
                
                cout << "Token = " << input[current] << endl;
                cout << "STACK TOP VALUE = " << stk.top() << endl;
                for(int i=prod.length()-1;i>=0;i--)
                {
                    stk.push(prod.substr(i,1));
                }
            }
            if (X == "$") return true;
        }
    }

    bool interpret()
    {   
        
        if (str2[curr] == '!' || str2[curr] == '_')
        {
            return false;
        }
        if (str2.find('=') == string::npos)
        {
            assigned = true;
        }    
        cout << "\n\n   ::::::::::::::::: New Sentence :::::::::::::::::   " << str2 << "\n\n\n";
        cout << "Token: " << getLex(str2.substr(curr,1)) << "           " << "Lexeme: " << str2[curr] << endl;
        stk.push("$"); stk.push("E");
        string a, X;
        for(;;) {
            a = str2[curr];
            X = stk.top();
            if (a == "=")
            {
                assigned = true;
                once = false;
            }
            //cout << "STACK TOP VALUE = " << X << endl;
            if (isTerminal(X) || X == "$") 
            {
                //cout << "\n__________________entering terminal check token =?= stack top_________________\n\n";
                if (a == X || (isalnum(a[0]) && X == "i")) 
                {   
                    //cout << "__________exiting terminal check token = stack top_________ \n";
                    //cout << "about to pop " << stk.top() << endl;
                    expansion.insert(expansion.begin(), stk.top());
                    expandTerms();
                    expansion.clear();
                    stk.pop(); 
                    curr++;
                    if (!assigned)
                    {
                        assigned = true;
                    }
                    cout << "\nToken = " << getLex(str2.substr(curr,1)) << "           " << "Lexeme: " << str2[curr] << "\n";
                    //str3 = "";
                }   
                else 
                {
                    if (a == ";")
                    {
                        expandTerms();
                        expansion.clear();
                        //cout << "end of sentence semicolon matches end of expression : SUCCESS \n";
                        curr = 0;
                        assigned = false;
                        once = true;
                        return true;
                    }
                    cout << "interpreted value = false\nreason : token " << a << " does not match stack top " << X << endl << endl;
                    return false;
                }
            } 
            else
            {
                
                string prod = table(X,a);
                if (prod == "skip")
                {
                    while(!stk.empty())
                    {
                        stk.pop();
                    }
                    
                    expansion.clear();
                    stk.push("$");
                    stk.push("E");
                    assigned = true;
                    curr++;
                    cout << "\nToken = " << getLex(str2.substr(curr,1)) << "           " << "Lexeme: " << str2[curr] << "\n";
                    continue;
                }
                //cout << "about to pop " << stk.top() << endl;
                expansion.insert(expansion.begin(), stk.top());
                expandTerms();
                expansion.clear();
                stk.pop();
                //str3 = "";
                //cout << "NEW STACK TOP VALUE = " << stk.top() << endl;
                if (prod == "nil") 
                {
                    cout << "interpreted value = false\n";
                    return false;
                }
                for(int i=prod.length()-1;i>=0;i--)
                {
                    stk.push(prod.substr(i,1));
                    expansion.insert(expansion.begin(), prod.substr(i,1));                   
                    //cout << "pushed stack value for " << a << " : "<< stk.top() << endl;
                    if ((a == "+" || a == "-" || a == "*" || a == "/") && (prod[i] == 'e' || prod[i] == 't'))
                    {
                        expansion.insert("~");  // for EPSILON use
                    }
                } 
                //str3 += prod;
                if (prod == "")
                {
                    expansion.insert("~");      // for EPSILON use
                }
                //expandTerms();
                //expansion.clear();
            }
            if (X == "$" || X == ";") {
                    cout << boolalpha << "interpreted value = true\n";return true;}
        }
    }

    vector<string> expandTerms()
    {
        op = "";
        //cout << "expanding terms for " << str2[curr] << " : expected number of prod tokens = " << expansion.size() << endl;;
        vector<string> tmp;
        //debug line //cout << str3 << endl;
        for (string st : expansion)
        {
            /*switch (st[0])
            {
                case '+' :  

                case '-' :

                case '*' :
                
                case '/' :

                case '~' :

                default : break;
            }*/
            //cout << "current PROD RULE is " << st << endl;
            tmp.push_back(convert(st));
            
        }
        expanded = tmp;
        return tmp;
    }
};

