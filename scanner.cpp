// TINY Language Compiler
// compiler: g++ -o scanner .exe scanner.cpp
// Run:scanner.exe input.txt output.txt
/* Team Memeber:
  * Shahd mohamed abdelsalam      2100898
  * Kareem Mohamed Reda Mostafa   2101852
  * Demiana Ayman Gerges          2100590
  * Mariam Hany Mahmoud Noureldin 2100437 
  * Tuqa 
*/
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <unordered_map>
#include <vector>
using namespace std;
// Token tyoes we need to recognize

enum TokenType{
  SEMICOLON,IF,THEN, END, REPEAT, UNTIL, IDENTIFIER, ASSIGN, READ, WRITE, LESSTHAN, EQUAL, PLUS, MINUS, MULT, DIV, OPENBRACKET, CLOSEDBRACKET, NUMBER, UNKNOWN, END_OF_FILE // just increase two enums UNKNOWN, END_OF_FILE
};

// Structure to store token information
struct Token
{
  string value; // The actual text
  TokenType type; // the token type
};

class TinyScanner{
  private:
  string sourceCode; // stores the entire input
  size_t currentPosition ; // this the current reading position in source code
  unordered_map<string,TokenType> keywords;

  // Initialize reserved keywords
  void setupKeywords(){
    keywords={
      {"if",IF},{"then",THEN},{"end",END},
      {"repeat",REPEAT},{"until",UNTIL},
      {"read",READ},{"write",WRITE}
    };
  }

  // get current character 
  char getCurrentChar(){
    if(currentPosition >= sourceCode.length())return '\0';
    return sourceCode[currentPosition];
  }

  // look at next char without moving
  char peekNextChar(){
    if(currentPosition+1>=sourceCode.length())return '\0';
    return sourceCode[currentPosition+1];
  }

  // move to next char 
  void moveNext(){
    if(currentPosition < sourceCode.length())currentPosition++;
  }

  // skip spaces , tabs, newlines 
  void skipWhitespace(){
    while (currentPosition < sourceCode.length() && isspace(getCurrentChar()))
    {
      moveNext();
    }
  }

  // skip comments { } and //  
  
  void skipComments(){
    //  comments { ... }
    if(getCurrentChar() == '{'){
      moveNext();
      while(currentPosition < sourceCode.length() && getCurrentChar() != '}'){
        moveNext();
      }
      if (getCurrentChar()=='}') moveNext();
    }
    // line comments //
    else if(getCurrentChar() == '/' && peekNextChar() == '/'){
      while (currentPosition< sourceCode.length() && getCurrentChar() != '\n')
      {
        moveNext();
      }
    } 
  }

  //check if char is a letter
  bool isLetter(char c){
    return isalpha(static_cast<unsigned char>(c));
  }

  // check if char is a digit
  bool isDigit(char c){
    return isdigit(static_cast<unsigned char>(c));
  }

  // check if char is letter or digit
  bool isAlphaNumeric(char c){
    return isLetter(c) || isDigit(c);
  }

  // read a number token 
  Token readNumber(){
    string numberStr;
    while (currentPosition < sourceCode.length() && isDigit(getCurrentChar()))
    {
      numberStr+=getCurrentChar();
      moveNext();
    }
    return {numberStr,NUMBER};
    
  }

  // read an identifier or keyword
  Token readIdentifier(){
    string identifierStr;
    while (currentPosition<sourceCode.length() && isAlphaNumeric(getCurrentChar()))   
    {
      identifierStr+=getCurrentChar();
      moveNext();
    }

    // convert to lowercase for case-insensitive keyword matching
    string lowerStr = identifierStr;
    for (char& c:lowerStr ) c=tolower(c);

    // chekc oif it's a reserved keyword
    auto it = keywords.find(lowerStr);
    if(it != keywords.end()){
      return {identifierStr,it->second};
    }
    return {identifierStr,IDENTIFIER};
    
  }

  public:
  // takes the source code as input
  TinyScanner(const string &code):sourceCode(code),currentPosition(0){
    setupKeywords();
  }

  // main scanning func - converts source code to tokens 
  vector<Token> scanAllTokens(){
    vector<Token> tokens;
    while(currentPosition < sourceCode.length()){
      // skip whitespace
      skipWhitespace();
      if(currentPosition >= sourceCode.length() )break;

      char currentChar = getCurrentChar();
      
      // skip comments
      if(currentChar == '{'||(currentChar == '/' && peekNextChar() == '/')){
        skipComments();
        continue;
      }

      // identifiers and keywords
      if(isLetter(currentChar)){
        tokens.push_back(readIdentifier());
      }
      // numbers
      else if(isDigit(currentChar)){
        tokens.push_back(readNumber());
      }
      // special char and operators
      else{
        switch (currentChar)
        {
          case ':':
            if(peekNextChar() =='='){
              tokens.push_back({string(1,':='),ASSIGN});
              moveNext();
              moveNext();
            }else{
              tokens.push_back({string(1,currentChar),UNKNOWN});
            }
            break;
          case ';':tokens.push_back({string(1,';'),SEMICOLON});moveNext();break;
          case '<':tokens.push_back({string(1,'<'),LESSTHAN});moveNext();break;
          case '=':tokens.push_back({string(1,'='),EQUAL});moveNext();break;
          case '+':tokens.push_back({string(1,'+'),PLUS});moveNext(); break;
          case '-':tokens.push_back({string(1,'-'),MINUS});moveNext();break;
          case '*':tokens.push_back({string(1,'*'),MULT});moveNext();break;
          case '/':tokens.push_back({string(1,'/'),DIV});moveNext();break;
          case '(':tokens.push_back({string(1,'('),OPENBRACKET});moveNext();break;
          case ')':tokens.push_back({string(1,')'),CLOSEDBRACKET});moveNext();break;
        
          default:
            tokens.push_back({string(1,currentChar),UNKNOWN});
            moveNext();
            break;
        }
      }
    }
    // add end of file marker
    tokens.push_back({"",END_OF_FILE});
    return tokens;
  }
  // convert token to string for output
  static string getTokenTypeName(TokenType type){
    switch(type){
      case SEMICOLON:return "SEMICOLON";
      case IF:return "IF";
      case THEN: return "THEN";
      case END:return "END";
      case REPEAT:return "REPEAT";
      case UNTIL:return "UNTIL";
      case IDENTIFIER:return "IDENTIFIER";
      case ASSIGN:return "ASSIGN";
      case READ:return "READ";
      case WRITE:return "WRITE";
      case LESSTHAN:return "LESSTHAN";
      case EQUAL:return "EQUAL";
      case PLUS: return "PLUS";
      case MINUS:return "MINUS";
      case MULT:return "MULT";
      case DIV:return "DIV";
      case OPENBRACKET:return "OPENBRACKET";
      case CLOSEDBRACKET:return "CLOSEDBRACKET";
      case NUMBER :return "NUMBER";
      case UNKNOWN: return "UNKNOWN";
      case END_OF_FILE: return "END_OF_FILE";
      default: return "UNKNOWN";
       
    }
  }
};

// helper func to read entire file 
string readFileToString(const string & filename){
  ifstream file(filename);
  if(!file.is_open()){
    cerr<<"Error: Cannot open file"<<filename<<endl;
    return "";
  }
  string content((istreambuf_iterator<char>(file)),istreambuf_iterator<char>());
  file.close();
  return content;
}

// helpr func to write tokens to file
void writeTokensToFile(const vector<Token> & tokens,const string & filename){
  ofstream file(filename);
  if(!file.is_open()){
    cerr<<"Error: Cannot create file "<<filename<<endl;
    return ;
  }
  file<<"List of (tokenvalue, tokentype):\n";
  file<<"================================\n";

  for(const auto& token:tokens){
    if(token.type != END_OF_FILE){
      file<<token.value<<" , "<<TinyScanner::getTokenTypeName(token.type)<<endl;
    }
  }
  file.close();
}

int main(int argc, char* argv[]){
  cout<<"=== TINY Language Scanner ===\n";

  string inputFile="input.txt";
  string outputFile="output.txt";
  
  if(argc >= 2)inputFile=argv[1];
  if(argc>=3)outputFile=argv[2];

  cout<<"Input file: "<<inputFile<<endl;
  cout<<"Output file: "<<outputFile<<endl;

  // read input file 
  string sourceCode=readFileToString(inputFile);
  if(sourceCode.empty()){
    cerr<<"Error: Input file is empty or not found \n";
    return 1;
  }
  cout<<"\nSource code:\n"<<sourceCode<<"\n"<<endl;

  // create scanner and process code
  TinyScanner scanner(sourceCode);
  vector<Token> tokens = scanner.scanAllTokens();

  // write results to file 
  writeTokensToFile(tokens,outputFile);

  cout<<"Scanning completed successfully!\n";
  cout<<"Found "<<(tokens.size()-1)<<" tokens.\n";
  cout<<"Results saved to: "<<outputFile<<endl;

  return 0;
}
