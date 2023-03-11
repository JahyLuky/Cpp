#ifndef __PROGTEST__

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <array>
#include <cassert>

using namespace std;
#endif /* __PROGTEST__ */

bool is_number(const string &phone) {
    for (char const &digit: phone) {
        if (!isdigit(digit))
            return false;
    }
    return true;
}

int extract_to_words(ifstream &ifs, istringstream &iss, vector<string> &data, vector<string> &phones) {
    string word;
    int in = 0;
    // extract lines to words
    while (iss >> word) {
        in++;
        // every 3rd word is phone number
        if (in % 3 == 0) {
            if (is_number(word) && word[0] != '0' && word.length() == 9) {
                phones.push_back(word);
            } else {
                ifs.close();
                return false;
            }
        } else {
            data.push_back(word);
        }
    }
    return in;
}

bool read_file(ifstream &ifs, vector<string> &data, vector<string> &list_names, vector<string> &phones) {
    string names, word;
    bool flag = true;
    int in = 0;
    // read lines from file
    while (getline(ifs, names)) {
        in = names.length();
        if (in == 0) {
            flag = false;
            continue;
        }
        // flag -> reading phone list with names
        if (flag) {
            istringstream iss(names);
            in = extract_to_words(ifs, iss, data, phones);
            if (in != 3) {
                ifs.close();
                return false;
            }
            // reading list of questions
        } else {
            istringstream iss(names);
            in = 0;
            while (iss >> word) {
                in++;
                list_names.push_back(word);
            }
            if (in != 1) {
                ifs.close();
                return false;
            }
        }
    }
    ifs.close();
    return true;
}

bool report(const string &fileName, ostream &out) {
    ifstream ifs;
    ifs.open(fileName);
    if (ifs.fail())
        return false;

    vector<string> data;
    vector<string> list_names;
    vector<string> phones;
    if (ifs.is_open()) {
        if (!read_file(ifs, data, list_names, phones)) {
            return false;
        }
    }
    // find given names in data (given phone list with names)
    int cnt = 0;
    for (size_t i = 0; i < list_names.size(); ++i) {
        for (size_t j = 0; j < data.size(); ++j) {
            if (data[j] == list_names[i]) {
                cnt++;
                // we found match at first name -> skip surname
                if (j % 2 == 0) {
                    j++;
                }
                out << data[j - 1] << " " << data[j] << " " << phones[j / 2] << "\n";
                cout << data[j - 1] << " " << data[j] << " " << phones[j / 2] << "\n";
            }
        }
        out << "-> " << cnt << "\n";
        cout << "-> " << cnt << "\n";
        cnt = 0;
    }
    return true;
}

#ifndef __PROGTEST__

int main() {
    ostringstream oss;
    oss.str("");
    assert (report("tests/test0_in.txt", oss) == true);
    assert (oss.str() ==
            "John Christescu 258452362\n"
            "John Harmson 861647702\n"
            "-> 2\n"
            "-> 0\n"
            "Josh Dakhov 264112084\n"
            "Dakhov Speechley 865216101\n"
            "-> 2\n"
            "John Harmson 861647702\n"
            "-> 1\n");
    oss.str("");
    assert (report("tests/test1_in.txt", oss) == false);
    oss.str("");
    assert (report("tests/test2_in.txt", oss) == false);
    return 0;
}

#endif /* __PROGTEST__ */