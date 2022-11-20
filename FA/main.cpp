#include <bits/stdc++.h>

using namespace std;

// ifstream fin("FA.in");

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
};

int main() {
  FiniteAutomata fa = FiniteAutomata("FA.in");
  fa.read();
  fa.isCorrect("123");
  return 0;
}