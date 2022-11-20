#include <bits/stdc++.h>
using namespace std;

ifstream read_keywords("keywords.in");
ifstream read_input("input.txt");
ofstream write_output("output.txt");

ifstream read_identifiers("identifiers.txt");
ifstream read_constants("constants.txt");
class Node {
  public: 
    string value;
    Node* next;
    int index;

    Node(string value, int index) {
        this->value = value;
        this->next = NULL;
        this->index = index;
    }
};

class LinkedList {
  public:
    Node* head;
    LinkedList() {
        this->head = NULL;
    }

    bool insert(string value, int index) {
        if(find(value) != -1) return false;

        Node* node = new Node(value, index);
        if (this->head == NULL) {
            this->head = node;
        } else {
            Node* current = this->head;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = node;
        }
        return true;
    }

    int find(string value) {
        Node* current = this->head;
        while (current != NULL) {
            if (current->value == value) {
                return current->index;
            }
            current = current->next;
        }

        return -1;
    }

    void print() {
        Node *current = this->head;
        while (current != NULL) {
            cout << current->value << " " << current->index << endl;
            current = current->next;
        }
    }
};

class HashTable {
  private: 
    int size;
    int numberOfElements;
    LinkedList *table;
  public:
    
    HashTable() {
        this->size = 100;
        this->numberOfElements = 0;
        this->table = new LinkedList[size];
    }

    void resize() {
        int newSize = this->size * 2;
        LinkedList *newTable = new LinkedList[newSize];
        for (int i = 0; i < this->size; i++) {
            Node *current = this->table[i].head;
            while (current != NULL) {
                int hash = computeHash(current->value, newSize);
                newTable[hash].insert(current->value, current->index);
                current = current->next;
            }
        }
        this->size = newSize;
        this->table = newTable;
    }

    int computeHash(string value, int size) {
        int sum = 0;
        for (int i = 0; i < value.length(); i++) {
            sum += value[i];
        }
        return sum % this->size;
    }

    int find(string value) {
        int hash = computeHash(value, this->size);
        return this->table[hash].find(value);
    }

    bool insert(string value) {
        if(this->find(value) != -1) return false;
        int hash = computeHash(value, this->size);
        this->table[hash].insert(value, this->numberOfElements);
        this->numberOfElements++;

        if(this->numberOfElements >= this->size) {
            this->resize();
        }

        return true;
    }

    void print() {
        for (int i = 0; i < this->size; i++) {
            this->table[i].print();
        }
    }

    int getNumberOfElements() {
        return this->numberOfElements;
    }
};
class CustomNode {
    public:
        pair<int, int> value;
        CustomNode* next;

        CustomNode(pair<int, int> value) {
            this->value = value;
            this->next = NULL;
        }
};

class List{
    public:
        CustomNode* head;
        List() {
            this->head = NULL;
        }

        void insert(pair<int, int> value) {
            CustomNode* node = new CustomNode(value);
            if (this->head == NULL) {
                this->head = node;
            } else {
                CustomNode* current = this->head;
                while (current->next != NULL) {
                    current = current->next;
                }
                current->next = node;
            }
        }

        void print() {
            CustomNode *current = this->head;
            while (current != NULL) {
                write_output << current->value.first << " " << current->value.second << endl;
                current = current->next;
            }
        }
};

class FiniteAutomata {
  public:
    vector<string> states;
    vector<char> alphabet;
    string initial_state;
    vector<string> final_states;
    vector<tuple<string, char, string>> transitions;
    fstream fin;

    FiniteAutomata(){

    }

    FiniteAutomata(string file){
        fin.open(file, ios_base::in);
        read();
    }

    // read a line and return string without label before equal
    string read_line(){
        string line;
        getline(fin, line);
        while (line[0] != '=') {
            line.erase(line.begin());
        }
        line.erase(line.begin());
        return line;
    }


    void read_states() {
        stringstream ss(read_line());
        string state;

        while(getline(ss, state, ',')) {
          state.erase(remove(state.begin(), state.end(), ' '), state.end());
          states.push_back(state);
        }
    }

    void read_alphabet() {
        stringstream ss(read_line());
        string symbol;
        while(getline(ss, symbol, ',')) {
          symbol.erase(remove(symbol.begin(), symbol.end(), ' '), symbol.end());
          alphabet.push_back(symbol[0]);
        }
    }

    void read_initial_state() {
        string line = read_line();
        line.erase(remove(line.begin(), line.end(), ' '), line.end());
        initial_state = line;
    }

    void read_final_states(){
        stringstream ss(read_line());
        string state;
        while(getline(ss, state, ',')) {
          state.erase(remove(state.begin(), state.end(), ' '), state.end());
          final_states.push_back(state);
        }
    }

    tuple<string, char, string> read_one_transition(){
        string line;
        getline(fin, line);
        while (line[0] != '(') {
            line.erase(line.begin());
        }
        line.erase(line.begin());
        stringstream ss(line);
        string state;
        getline(ss, state, ',');
        state.erase(remove(state.begin(), state.end(), ' '), state.end());
        string symbol;
        getline(ss, symbol, ')');
        symbol.erase(remove(symbol.begin(), symbol.end(), ' '), symbol.end());
        
        getline(ss, line, '>');
        getline(ss, line, '-');
        line.erase(remove(line.begin(), line.end(), ' '), line.end());
        return make_tuple(state, symbol[0], line);
     }


    void read_transitions() {
        read_line();

        while (fin.peek() != EOF) {
            transitions.push_back(read_one_transition());
        }
    }

    void read() {
        read_states();
        read_alphabet();
        read_initial_state();
        read_final_states();
        read_transitions();
    }

    bool isCorrect(string word) {
        string current_state = initial_state;
        for (int i = 0; i < word.size(); i++) {
            bool found = false;

            for (int j = 0; j < transitions.size(); j++) {
                if (get<0>(transitions[j]) == current_state && get<1>(transitions[j]) == word[i]) {
                    current_state = get<2>(transitions[j]);
                    found = true;
                    break;
                }
            }

            if (!found) {
                return false;
            }
        }

        for (int i = 0; i < final_states.size(); i++) {
            if (final_states[i] == current_state) {
                return true;
            }
        }

        return false;
    }

    void print() {
        cout << "STATES = ";
        for (int i = 0; i < states.size(); i++) {
            cout << states[i];
            if (i != states.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
        cout << "ALPHABET = ";
        for (int i = 0; i < alphabet.size(); i++) {
            cout << alphabet[i];
            if (i != alphabet.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
        cout << "INITIAL_STATE = " << initial_state << endl;
        cout << "FINAL_STATES = ";
        for (int i = 0; i < final_states.size(); i++) {
            cout << final_states[i];
            if (i != final_states.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
        cout << "TRANSITIONS = " << endl;
        for (int i = 0; i < transitions.size(); i++) {
            cout << "(" << get<0>(transitions[i]) << ", " << get<1>(transitions[i]) << ") -> " << get<2>(transitions[i]) << endl;
        }
    }

    void menu(){
        string word;
        while(word != "*"){
            cout << "Enter word: ";
            cin >> word;
            if (word == "*") {
                break;
            }
            if (isCorrect(word)) {
                cout << "Word is correct" << endl;
            } else {
                cout << "Word is not correct" << endl;
            }
        }
    }
};

class SymbolTable {
    public: 
        HashTable symbolTable;
        HashTable reservedKeywords; 
        List internalNormalForm;
        FiniteAutomata identifiers;
        FiniteAutomata constants;


        SymbolTable() {
            identifiers = FiniteAutomata("identifiers.txt");
            constants = FiniteAutomata("constants.txt");
            readReservedKeywords();
        }

        void print() {
            this->symbolTable.print();
        }

    void readReservedKeywords(){
        string line;
        while (getline(read_keywords, line)) {
            line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
            this->reservedKeywords.insert(line);
        }
    } 

    bool isVariableName(string value) {

        return identifiers.isCorrect(value);
    }

    bool isValue(string value) {
        if(value == "true" || value == "false") return true;

        return constants.isCorrect(value);
    }

    bool isSeparator(char charValue) {
        string value(1, charValue);
        return value == "(" || value == ")" || value == "{" || value == "}" || value == ";" || value == "=" || value == " " || value == ",";
    }

    void parse() {
        int numberOfErrors = 0;
        string line;
        int lineIndex = 1;
        while (getline(read_input, line)) {
            string value = "";
            for (int i = 0; i <= line.length(); i++) {
                if (isSeparator(line[i]) || i == line.length()) {
                    if (value != "") {
                        int index = reservedKeywords.find(value);
                        if (index == -1) {
                            if (isVariableName(value)) {
                                symbolTable.insert(value);
                                internalNormalForm.insert(make_pair(reservedKeywords.find("IDENTIFIER"), symbolTable.find(value)));
                            }
                            else if(isValue(value)) {
                                 symbolTable.insert(value);
                                internalNormalForm.insert(make_pair(reservedKeywords.find("CONST"), symbolTable.find(value)));
                                
                            }
                            else{
                                numberOfErrors++;
                                write_output << "Error on line " << lineIndex << ": " << value << " is not a valid variable name or value" << endl;
                            }
                        }
                        else{
                            internalNormalForm.insert(make_pair(index, -1));
                        }
                    }
                    if(isSeparator(line[i])){
                        string separator(1, line[i]);
                        int index = reservedKeywords.find(separator);
                        if(index >= 0){
                            internalNormalForm.insert(make_pair(index, -1));
                        }
                        
                    }
                    value = "";
                } else {
                    value += line[i];
                }
            }

            lineIndex++;
        }

        if(numberOfErrors == 0){
            internalNormalForm.print(); 
        }
    }
};

int main() {
    SymbolTable symbolTable;

    symbolTable.parse();

    // FiniteAutomata identifiers("identifiers.txt");
    // identifiers.print();
    // identifiers.menu();
    return 0;

}