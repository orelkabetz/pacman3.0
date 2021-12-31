#include "io_utils.h"

class InputParser {
private:
public:
    int mode = NULL;
    vector <string> tokens;
    InputParser(int& argc, char** argv) {
        for (int i = 1; i < argc; ++i)
            this->tokens.push_back(string(argv[i]));
    }

    void checkMode()
    {
        if (cmdOptionExists("-save")) // maybe to add if it finds both silent+save+load
            mode = Mode::SAVE;
        else if (cmdOptionExists("-load"))
        {
            if (cmdOptionExists("[-silent]"))
                mode = Mode::SILENT;
            else
                mode = Mode::LOAD;
        }
        else
        {
            cout << "No valid Mode enterd!";
            return; //add exceptions
        }
    }
 
    bool cmdOptionExists(const string& option) const {
        return find(this->tokens.begin(), this->tokens.end(), option)
            != this->tokens.end();
    }

    void printFirstToken() {
        string tmp = tokens[0];
        cout << tmp << endl;
    }

    const string& getCmdOption(const string& option) const {
        vector<string>::const_iterator itr;
        itr = find(this->tokens.begin(), this->tokens.end(), option);
        if (itr != this->tokens.end() && ++itr != this->tokens.end()) {
            return *itr;
        }
        static const string empty_string("");
        return empty_string;
    }

};