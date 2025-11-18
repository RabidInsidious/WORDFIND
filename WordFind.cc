//Marc-Alexandre Martin-Lauzer
//martinlauzerm2@winthop.edu

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <array>
#include <cstddef>
#include <cctype>

using namespace std;

// Function declaration
void searchWord(const vector<vector<char>> &matrix, const string &word,
                const vector<pair<int, int>> &directions,
                const vector<string> &directionNames);

int main(int argc, char *argv[]) {
   // checking for correct number of arguments
    if (argc != 2 && argc != 3) {
        cout << "Usage: " << argv[0] << " <filename> [wordlist]" << endl;
        return 1;
    }

   // variables
    string filename;
    string word;
    vector<string> wordlist;

    // if 2 args assign only filename
    if (argc == 2) {
        filename = argv[1];
    }

    // if 3 args read in wordlist
    if (argc == 3) {
        filename = argv[1];
        ifstream wordsinput(argv[2]);
        if (!wordsinput.is_open()) {
            cout << "Word list file not read successfully. It does not work!" << endl;
            return 1;
        }
        while (getline(wordsinput, word)) {
            if (!word.empty()) {
                wordlist.push_back(word);
            }
        }
        wordsinput.close();
    }

   // direction names and their corresponding row/column changes
    vector<string> directionNames = {
        "North", "Northeast", "East", "Southeast",
        "South", "Southwest", "West", "Northwest"};

   // pairs for row and column changes
    vector<pair<int, int>> directions = {
        {-1, 0}, {-1, 1}, {0, 1}, {1, 1},
        {1, 0}, {1, -1}, {0, -1}, {-1, -1}};

    // reading in file name for word matrix
    ifstream file(filename);
    // checking if file works
    if (!file.is_open()) {
        cout << "File not read successfully. It does not work!" << endl;
        return 1;
    }

    // making a 2D vector
    vector<vector<char>> matrix;
    string line;

    // filling in the 2D vector
    while (getline(file, line)) {
        istringstream iss(line);
        // making another vector for rows
        vector<char> row;
        char ch;
        // stringstreaming input into char variable and putting into row vector
        while (iss >> ch) {
            if (isalpha(static_cast<unsigned char>(ch))) {
                row.push_back(ch);
            }
        }

        // if no input for this row then shift in matrix column
        if (!row.empty()) {
            matrix.push_back(row);
        }
    }
    file.close();

    // if 3 args read from wordlist
    if (argc == 3) {
        for (const auto &w : wordlist) {
            searchWord(matrix, w, directions, directionNames);
        }
    }
    // if 2 args prompt user repeatedly
    else if (argc == 2) {
        while (true) {
            cout << "What word do you want to search for (press enter to exit)? ";
            getline(cin, word);
            if (word.empty()) break;
            searchWord(matrix, word, directions, directionNames);
        }
    }

    // debugging matrix input
    /*for (size_t i = 0; i < matrix.size(); ++i){
       for (size_t j = 0; j < matrix[i].size(); ++j){
          for (const auto& dir:directions){
             if ((matrix[i][j] == word[0])||(matrix[i][j] == reversedword[0])){
                  storedPositions[i].push_back(j);
             }else{
                continue;
             }
           }
        }
      } */

    /*
   for (const auto& row:matrix){
      for (char c:row){
         cout << c << ' ';
      }
      cout << '\n';
   }
   */

    return 0;
}

// Function definition
void searchWord(const vector<vector<char>> &matrix, const string &word,
                const vector<pair<int, int>> &directions,
                const vector<string> &directionNames) {
    // helper lambda for lowercase conversion
    auto toLowerChar = [](char ch) {
        return static_cast<char>(tolower(static_cast<unsigned char>(ch)));
    };

    // convert word to lowercase
    string lowerWord;
    for (char ch : word) lowerWord += toLowerChar(ch);

    bool found = false;

    // searching for first letter of word in the matrix
    for (size_t r = 0; r < matrix.size(); ++r) {
        for (size_t c = 0; c < matrix[r].size(); ++c) {
            if (toLowerChar(matrix[r][c]) == lowerWord[0]) {
                for (size_t d = 0; d < directions.size(); ++d) {
                    // initialize direction row and column changes
                    int dr = directions[d].first;
                    int dc = directions[d].second;

                    bool match = true;
                    for (size_t k = 1; k < lowerWord.size(); ++k) {
                        // calculate new row and column positions
                        int newr = static_cast<int>(r) + dr * static_cast<int>(k);
                        int newc = static_cast<int>(c) + dc * static_cast<int>(k);
                        // check boundaries of new positions
                        if (newr < 0 || newr >= static_cast<int>(matrix.size()) ||
                            newc < 0 || newc >= static_cast<int>(matrix[newr].size())) {
                            match = false;
                            break;
                        }
                        // check if character matches (case-insensitive)
                        if (toLowerChar(matrix[newr][newc]) != lowerWord[k]) {
                            match = false;
                            break;
                        }
                    }

                    // if match found print the result and stop searching
                    if (match) {
                        cout << "Found " << word << " starting at ("
                             << r + 1 << ", " << c + 1 << ") going "
                             << directionNames[d] << endl;
                        found = true;
                        return; // stop after first occurrence
                    }
                }
            }
        }
    }

    // if not found, indicate it
    if (!found) {
        cout << word << " was not found" << endl;
    }
}
