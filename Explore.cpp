#include "RedBlackTree.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include <cctype>
#include <stdexcept>
#include <functional>
using namespace std;

namespace {
  void printWelcomeMessage() {
    cout << "Welcome to the interactive red/black tree environment." << endl;
    cout << "You can issue the following commands: " << endl;
    cout << endl;
    cout << "  i value: insert the given value." << endl;
    cout << "  c value: return whether the given value is in the tree." << endl;
    cout << "  r value: return the rank of the given value." << endl;
    cout << "  s index: returns the element at the given index." << endl;
    cout << "  p:       prints debug information." << endl;
    cout << "  q:       quit this program." << endl;
    cout << endl;
  }
  
  string getLine(const string& prompt) {
    cout << prompt;
    string result;
    
    /* Read a line. If the user CTRL+D's out, print a newline to be courteous. */
    if (!getline(cin, result)) {
      cout << endl;
      exit(EXIT_SUCCESS);
    }
    return result;
  }
  
  /* Trims trailing comments, if they exist. */
  string trim(string input) {
    size_t hashIndex = input.find('#');
    if (hashIndex != string::npos) input = input.substr(0, hashIndex);
    return input;
  }
  
  /* Exception type representing a parse error. */
  struct ParseError: public exception {
  public:
    ParseError(const string& parsed) : exception() {
      this->parsed = "Error parsing ";
      this->parsed += parsed;
      this->parsed += ".";
    }
    
    virtual const char* what() const noexcept override {
      return parsed.c_str();
    }
  
  private:
    string parsed;
  };
  
  template <typename T> T extract(istream& source, const string& what) {
    T result;
    source >> result;
    
    if (!source) {
      throw ParseError(what);
    }
    
    char leftover;
    if (source >> leftover) {
      throw ParseError(what);
    }
    
    return result;
  }
  
  int parseKey(istream& source) {
    return extract<int>(source, "value");
  }
  
  size_t parseRank(istream& source) {
    return extract<size_t>(source, "index");
  }
  
  void execute(function<void()> fn) {
    try {
      fn();
    } catch (const exception& e) {
      cerr << "Error: " << e.what() << endl;
    }
  }
  
  void executeCommand(RedBlackTree& t, const string& line) {
    istringstream input(trim(line));
    
    /* Read the command letter; if we can't, the line is empty and we don't
     * need to do anything.
     */
    char command;
    input >> command;
    if (!input) return;
    
    command = tolower(command);
    if (command == 'i') {
      execute([&] {
        int key = parseKey(input);
        cout << boolalpha << t.insert(key) << endl;
      });
    } else if (command == 'c') {
      execute([&] {
        int key = parseKey(input);
        cout << boolalpha << t.contains(key) << endl;
      });
    } else if (command == 'r') {
      execute([&] {
        int key = parseKey(input);
        cout << t.rankOf(key) << endl;
      });
    } else if (command == 's') {
      execute([&] {
        try {
          size_t rank = parseRank(input);
          cout << t.select(rank) << endl;
        } catch (const runtime_error& e) {
          cout << "std::runtime_error thrown: " << e.what() << endl;
        }
      });
    } else if (command == 'p') {
      t.printDebugInfo();
    } else if (command == 'q') {
      exit(EXIT_SUCCESS);
    } else {
      cerr << "Unknown command." << endl;
    }
  }

  void runInteractively() {
    printWelcomeMessage();
    
    RedBlackTree t;
    while (true) {
      string command = getLine(">>> ");
      executeCommand(t, command);
    }
  }
  
  void runScriptFile(const string& filename) {
    ifstream input(filename);
    if (!input) {
      cerr << "Cannot open file \"" << filename << "\" for reading." << endl;
      exit(-1);
    }
    
    RedBlackTree t;
    for (string line; getline(input, line); ) {
      cout << ">>> " << line << endl;
      executeCommand(t, line);
    }
  }
}

int main(int argc, const char* argv[]) {
  if (argc == 1) {
    runInteractively();
  } else if (argc == 2) {
    runScriptFile(argv[1]);
  } else {
    cerr << "Usage: ./explore [optional-test-file]" << endl;
    return -1;
  }
}
