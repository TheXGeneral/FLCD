#include <bits/stdc++.h>
using namespace std;

ifstream read_keywords("keywords.in");
ifstream read_input("input.txt");
ofstream write_output("output.txt");

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

class SymbolTable {
    public: 
        HashTable symbolTable;
        HashTable reservedKeywords; 
        List internalNormalForm;

        SymbolTable() {
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
        for (int i = 0; i < value.length(); i++) {
            if (!isalpha(value[i])) {
                return false;
            }
        }
        return true;
    }

    bool isValue(string value) {
        if(value == "true" || value == "false") return true;

        for (int i = 0; i < value.length(); i++) {
            if (!isdigit(value[i])) {
                return false;
            }
        }

        return true;
    }

    bool isSeparator(char charValue) {
        string value(1, charValue);
        return value == "(" || value == ")" || value == "{" || value == "}" || value == ";" || value == "=" || value == " " || value == ",";
    }

    void parse() {
        int numberOfErrors = 0;
        string line;
        int lineIndex = 0;
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

    return 0;

}