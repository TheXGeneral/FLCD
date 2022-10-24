#include <bits/stdc++.h>
using namespace std;
ifstream fin("input.txt");

class Node {
  public: 
    string value;
    Node* next;
    Node(string value) {
        this->value = value;
        this->next = NULL;
    }
};

class LinkedList {
  public:
    Node* head;
    LinkedList() {
        this->head = NULL;
    }

    bool insert(string value) {
        if(find(value)) return false;

        Node* node = new Node(value);
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

    bool find(string value) {
        Node* current = this->head;
        while (current != NULL) {
            if (current->value == value) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    void print() {
        Node *current = this->head;
        while (current != NULL) {
            cout << current->value << " ";
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
                newTable[hash].insert(current->value);
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

    bool find(string value) {
        int hash = computeHash(value, this->size);
        return this->table[hash].find(value);
    }

    bool insert(string value) {
        if(this->find(value)) return false;
        int hash = computeHash(value, this->size);
        this->table[hash].insert(value);
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


void setReservedKeywords(HashTable& reservedKeywords){
  reservedKeywords.insert("create");
  reservedKeywords.insert("as");
  reservedKeywords.insert("Number");
  reservedKeywords.insert("Boolean");
  reservedKeywords.insert("repeatWithValues");
  reservedKeywords.insert("withStep");
  reservedKeywords.insert("using");
  reservedKeywords.insert("if");
  reservedKeywords.insert("else");
  reservedKeywords.insert("isGreaterThan");
  reservedKeywords.insert("isSmallerThan");
  reservedKeywords.insert("isGreaterOrEqualThan");
  reservedKeywords.insert("isSmallerOrEqualThan");
  reservedKeywords.insert("isNotEqual");
  reservedKeywords.insert("isEqual");
  reservedKeywords.insert("print");
  reservedKeywords.insert("read");
  reservedKeywords.insert("List");
  reservedKeywords.insert("of");
  reservedKeywords.insert("lengthOf");
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

void parse(HashTable reservedKeywords, HashTable &symbolTable) {
    string line;
    while (getline(fin, line)) {
        string value = "";
        for (int i = 0; i < line.length(); i++) {
            if (line[i] == ' ' || line[i] == ';' || line[i] == ',') {
                if (value != "") {
                    if (!reservedKeywords.find(value)) {
                        if (isVariableName(value) || isValue(value)) {
                            symbolTable.insert(value);
                        }
                    }
                }
                value = "";
            } else {
                value += line[i];
            }
        }
    }
}

int main() {
  HashTable reservedKeywords;
  HashTable symbolTable;

  setReservedKeywords(reservedKeywords);
  parse(reservedKeywords, symbolTable);

  symbolTable.print();

  return 0;
}