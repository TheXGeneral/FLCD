#include <bits/stdc++.h>


using namespace std;

ifstream fin("text.in");

class Grammar {
  public: 
    vector<string> setOfNonTerminals;
    vector<string> setOfTerminals;
    string startingSymbol;
    map<string, vector< vector<string> > > productions;

    // space separated on one line
    void readSetOfNonTerminals(){
      string line;
      getline(fin, line);
      stringstream ss(line);
      string token;
      while(ss >> token){
        if(token != " "){
          setOfNonTerminals.push_back(token);
        }
      }
    } 

    void readSetOfTerminals(){
      string line;
      getline(fin, line);
      stringstream ss(line);
      string token;
      while(ss >> token){
        if(token != " "){
          setOfTerminals.push_back(token);
        }
      }
    }

    void readProductions(){
        string line;
      
        while(getline(fin, line)){
          stringstream ss(line);
          string token;
          vector<string> production;
          string nonTerminal;
          while(ss >> token){
            if(token == "->"){
              nonTerminal = production[0];
              production.clear();
            } else if(token == "|"){
              productions[nonTerminal].push_back(production);
              production.clear();
            } else {
              production.push_back(token);
            }
          }
          productions[nonTerminal].push_back(production);

          // nonTerminal is not part setOfNonTerminals, throw error 
          if(find(setOfNonTerminals.begin(), setOfNonTerminals.end(), nonTerminal) == setOfNonTerminals.end()){
            throw("Error: Invalid nonTerminal");
          }
        }
    }

    int getProductionIndex(string nonTerminal, vector<string> production){
      int index = 0;
      for(auto production : productions[nonTerminal]){
        if(checkIfProductionsAreEqual(production, production)){
          return index;
        }
        index++;
      }
      return -1;
    }

    void readStartingSymbol(){
      string line;
      getline(fin, line);
      startingSymbol = line;
    }

    void parse(){
      readSetOfNonTerminals();
      readSetOfTerminals();
      readStartingSymbol();
      readProductions();
    }

    void print(){
      cout << "Non terminals: ";
      for(auto nonTerminal : setOfNonTerminals){
        cout << nonTerminal << " ";
      }
      cout << endl;
      cout << "Terminals: ";
      for(auto terminal : setOfTerminals){
        cout << terminal << " ";
      }
      cout << endl;
      cout << "Starting symbol: " << startingSymbol << endl;
      cout << "Productions: " << endl;
      for(auto nonTerminal : setOfNonTerminals){
        for(auto production : productions[nonTerminal]){
          cout << nonTerminal << " -> ";
          for(auto symbol : production){
            cout << symbol << " ";
          }
          cout << endl;
        }
      }
    }

    // check if 2 productions are equal 
    bool checkIfProductionsAreEqual(vector<string> firstProduction, vector<string> secondProduction){
      if(firstProduction.size() != secondProduction.size()){
        return false;
      }

      for(int i = 0; i< firstProduction.size(); i++){
        if(firstProduction[i] != secondProduction[i]){
          return false;
        }
      }

      return true;
    }

     vector<string> getProductionForSymbol(string symbol, int index){
      return productions[symbol][index];
    } 

    string terminalToString(vector<string> production){
      string terminal = "";
      for(auto symbol : production){
        terminal += symbol;
      }
      return terminal;
    }

    bool isNonTerminal(string symbol){
      return find(setOfNonTerminals.begin(), setOfNonTerminals.end(), symbol) != setOfNonTerminals.end();
    }

    string getStartingSymbol(){
      return startingSymbol;
    }
};

// expand, advance, momentary insuccess, back, another try, success

class RecursiveDescendantParser {
  private:
    vector<string> input;
    int inputIndex;
    stack<string> workingStack;
    string currentToken;
    vector<pair<string, vector<string>>> usedProductions;

  public:
    Grammar grammar;

    RecursiveDescendantParser(vector<string> input, Grammar new_grammar) : input(input), inputIndex(0), grammar(new_grammar){
      workingStack.push(grammar.startingSymbol);
    }

    void expand(string nonTerminal){
      int index = 0;
      vector<string> production = grammar.getProductionForSymbol(nonTerminal, index);
      usedProductions.push_back(make_pair(nonTerminal, production));
      for(int i = 0; i < production.size(); i++){
        workingStack.push(production[i]);
      }
    }

    bool advance(){
      if(workingStack.empty()){
        return false;
      }

      if(workingStack.top() == input[inputIndex]){
        workingStack.pop();
        inputIndex++;
        return true;
      }

      return false;
    }

    bool momentaryInsuccess(){
      if(workingStack.empty()){
        return false;
      }
      string symbol = workingStack.top();
      workingStack.pop();
      if(grammar.isNonTerminal(symbol)){
        int lastUsedProduction = grammar.getProductionIndex(symbol, usedProductions.back().second);
        vector<string> nextProduction = grammar.getProductionForSymbol(symbol, lastUsedProduction + 1);
        if(nextProduction.size() == 0){
          return false;
        }
        else{
          usedProductions.pop_back();
          usedProductions.push_back(make_pair(symbol, nextProduction));
          for(int i =  0; i < nextProduction.size(); i++){
            workingStack.push(nextProduction[i]);
          }
          return true;
        }
      }

      return false;
    }

    void back(){
      usedProductions.pop_back();
      workingStack = stack<string>();
    }

    void anotherTry(){
      workingStack = stack<string>();
      inputIndex = 0;
      usedProductions.clear();
    }

    bool success(){
      return workingStack.empty() && inputIndex == input.size();
    }

    void parse(){
      workingStack.push(grammar.startingSymbol);
      while(!success()){
        if(!advance()){
          if(!momentaryInsuccess()){
            if(usedProductions.size() > 0){
              back();
            }
            else{
              anotherTry();
            }
          }
        }
        else if(grammar.isNonTerminal(workingStack.top())){
          expand(workingStack.top());
        }
      }
      //print used productions
      for(auto production : usedProductions){
        cout << production.first << " -> ";
        for(auto symbol : production.second){
          cout << symbol << " ";
        }
        cout << endl;
      }
    }
};

vector<string> readFromFile(){
    vector<string> data;
    string line;
    ifstream read_code("input.txt");
    while(getline(read_code, line)) {
        stringstream ss(line);
        string token;
        while(ss >> token) {
            data.push_back(token);
        }
    }

   return data;
}

int main() {
    Grammar grammar;
    grammar.parse();
    vector<string> v = {"as_df", "0", "0", "0"};
    RecursiveDescendantParser parser(v, grammar);
    parser.grammar = grammar;
    parser.parse();

    return 0;
}