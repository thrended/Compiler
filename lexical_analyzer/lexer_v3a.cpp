/**********

Group Members:
Duc Nguyen
Wayne Lin

**********/
#include <unistd.h>
#include <stdlib.h>
#include <iterator>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <list>
#include <set>
#include <unordered_set>
#include <map>

using namespace std;

int main(){
    // declare variables
    ifstream infile;
    string s, rawfile, line, fileName = "";
    vector<char> bytes;
    char prev = '\0';
    int state = 0;  //  initial state

    //  define token groups and keywords
    static const unordered_set<string> keywords = {"int", "float", "bool", "True", "False",
                  "if", "else", "then", "endif", "endelse", "while", "whileend", "do",
                  "enddo", "for", "endfor", "STDinput", "STDoutput", "and", "or", "not"};

    static const unordered_set<char> ops = { '+', '-', '*', '/', '=', '<', '>', '%'};
    static const unordered_set<char> seps = { '{', '}', '[', ']', '(', ')', ',', '.', ';', ':' };
    static const unordered_set<char> ws = { '\n', '\r', '\t', ' ' };
    static const unordered_set<char> nums = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
    static const unordered_set<char> spec = { '!', '_' };
    //static const unordered_set<char> identifiers = { ''}

    // get data from user
    cout<<"\nPlease enter the name of the file: ";
    getline(cin, fileName);

    infile.open(fileName.c_str());

    // cannot open the file
    if(infile.fail())
    {
        cout<<"\n** ERROR - the file \""<<fileName<<"\" cannot be found!\n\n";
        exit(1);
    }

    cout << "TOKENS                       LEXEMES" << endl << endl;
     // display the file
    /*while(infile.eof()==0)
    {
        infile >> s;
        str2.append(s);
        cout << s;
    }*/
    bool flag = false;
    string curr;
    /*
    regex rgxNM("[0-9]+");
    regex rgxID("[a-zA-Z][a-zA-Z0-9]*");
    regex rgxINT("((?:^|\\s)([+-]?[[:digit:]]+(?:^\\.[[:digit:]]+)?)(?=$|\\s))");
    regex rgxDEC("((?:^|\\s)([+-]?[[:digit:]]+(?:^\\.[[:digit:]]+)?)(?=$|\\s))");
    regex rgxSP("/\\{|\\}|\\(|\\)|\\[|\\]|\\,|\\.|\\:|\\;/");
    */
    if (infile.is_open())
    {
        while (std::getline(infile, line, '\0')){
          state = 1;
          curr = "";
          rawfile.append(line);
          for(char byte : line){
            //cout << byte << "";
            switch(state){
              case 1  : // Default traversal
                if (find(ws.begin(), ws.end(), byte) != ws.end())
                {
                  //cout << "STATE 2";  // skipping past white spaces
                  continue;
                } else
                {
                  if (find(ops.begin(), ops.end(), byte) != ops.end())
                  {
                    cout << "OPERATOR  " << "         =         " << byte << endl;
                    curr = "";
                    state = 1;  // end of a token
                    continue;

                  } else if (find(seps.begin(), seps.end(), byte) != seps.end())
                  {
                    if (byte == '.' && find(nums.begin(), nums.end(), prev) == nums.end())
                    {
                      cout << "SEPARATOR " << "         =         " << byte << endl;  // STATE 4
                      curr = "";
                      state = 1; // begin process next token
                      continue;
                    }
                    flag = true;
                    state = 2;  // real number
                    continue;
                  } else if (find(nums.begin(), nums.end(), byte) != nums.end())
                  {
                    // beginning of a NUMBER
                    state = 2;
                    curr.push_back(byte);
                    continue;
                  } else if (find(spec.begin(), spec.end(), byte) != spec.end())
                  {
                    // skip block
                    state = 3;
                  } else
                  {
                    // beginning of word
                    state = 4;
                    curr.push_back(byte);
                    continue;
                  }

                }
              case 2  : // NUMBER
                if (find(ws.begin(), ws.end(), byte) != ws.end())
                {
                  if (!flag)
                  {
                    cout << "INTEGER   " << "         =         " << curr << endl;
                    curr = "";
                    flag = false;
                    state = 1;  // whitespace found, reset state
                    continue;
                  } else
                  {
                    cout << "REAL      " << "         =         " << curr << endl;
                    curr = "";
                    flag = false;
                    state = 1;  // whitespace found, reset state
                    continue;
                  }
                }
                else if (byte == '.')
                  {
                    flag = true;
                    curr.push_back(byte);
                    continue;
                  }
                else
                  {
                    curr.push_back(byte);
                    continue;
                  }
                /**/
              case 3  : // ! skip !
                if (byte != '!')
                {
                  continue;
                }
                state = 1;
              case 4 : // IDENTIFIER or KEYWORD
                flag = false;
                if (find(ws.begin(), ws.end(), byte) != ws.end())
                // end of word
                {
                  if (find(keywords.begin(), keywords.end(), curr) != keywords.end())
                  {
                    flag = true;
                  }
                  string tmp = (flag ? "KEYWORD   " : "IDENTIFIER");
                  cout << tmp << "         =         " << curr << endl;
                  curr = "";
                  state = 1;
                  continue;
                }
                else if (find(seps.begin(), seps.end(), byte) != seps.end())
                {
                  if (find(keywords.begin(), keywords.end(), curr) != keywords.end())
                  {
                    flag = true;
                  }
                  string tmp = (flag ? "KEYWORD   " : "IDENTIFIER");
                  cout << tmp << "         =         " << curr << endl;
                  cout << "SEPARATOR " << "         =         " << byte << endl;  // STATE 4
                  curr = "";
                  state = 1;
                  continue;
                }
                curr.push_back(byte);

            }
            prev = byte;
          }
        }
        /*while(file >> std::noskipws >> ascii){
          std::cout << (int) ascii << "\n";
        }*/
        /*while (infile.get(byte)))
        {
            rawfile.append(line.c_str());
        }*/
    }

    cout << endl;
    //cloes the file
    infile.close();
    infile.clear();

    /*
    auto it = sregex_iterator( rawfile.begin(), rawfile.end(), rgxNM );
    auto end = sregex_iterator();

    string str = " % 20 = a ();;;... wtf ;:;:;:;;. 1223456 []{}{}{}{}{{{}}} ! hello world ! num1 2 * 3 ok123; $3 @#omg int bool STDoutput 5.0";




    map<string,string> tokens {
        { "((?:^|\\s)([+-]?[[:digit:]]+(?:^\\.[[:digit:]]+)?)(?=$|\\s))" ,      "INTEGER   " },
        { "((?:^|\\s)([+-]?[[:digit:]]+(?:\\.[[:digit:]]+)?)(?=$|\\s))" ,       "REAL      " },
        { "((?:^|\\s)[a-zA-Z_]+[a-zA-Z0-9_@#$&]*(?![^ .,?!;\r\n]))" ,           "IDENTIFIER" },
        { "/\\{|\\}|\\(|\\)|\\[|\\]|\\,|\\.|\\:|\\;/{1}" ,                      "SEPARATOR " },
        { "/\\*|\\+|\\-|\\=|\\/|\\>|\\<|\\%/{1}" ,                              "OPERATOR  " },
        { "\\![^!]*\\!" ,                                                       "IGNORE    " }
    };

    // store results
    map< size_t, pair<string,string> > matches;

    for ( auto pat = tokens.begin(); pat != tokens.end(); ++pat )
    {
        //regex r("\\w+");
        state = 2;  //  iterative state
        regex rx(pat->first);
        auto it = sregex_iterator( str.begin(), str.end(), rx );
        auto end = sregex_iterator();
        state = 1;

        for (; it != end; ++it )
        {
          state = 2;
          matches[ it->position() ] = make_pair( it->str(), pat->second );

        }

    }
    state = 1;    //   waiting/idle state

    cout << "TOKENS                       LEXEMES" << endl;

    for ( auto match = matches.begin(); match != matches.end(); ++match )
    {
      state = 2;    //  iterative state
      auto lex = match->second.first;
      auto tok = match->second.second;
      if (tok == "IGNORE") continue;
      tok = (find(keywords.begin(), keywords.end(), lex) == keywords.end() ? tok : "KEYWORD   ");
      cout << tok << "         =         " << lex << endl;
    }

    state = 3;  //  final state
    */

}
