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

struct Entry {
  string firstName;
  string lastName;
  string phone;
};

bool report(const string &fileName, ostream &out) {
  // Open the input file
  ifstream input(fileName);

  // Check if the file was opened successfully
  if (!input.is_open()) {
    return false;
  }

  // Read the phonebook entries from the file
  vector<Entry> phonebook;
  string line;
  while (getline(input, line)) {
    // Stop reading when an empty line is encountered
    if (line.empty()) {
      break;
    }

    // Read the first name, last name, and phone number from the line
    istringstream iss(line);
    string firstName, lastName, phone;
    iss >> firstName >> lastName >> phone;

    // Check if the phone number is valid
    if (phone.length() != 9 || phone[0] == '0' || !isdigit(phone[0])) {
      return false;
    }

    // Add the entry to the phonebook
    phonebook.push_back({ firstName, lastName, phone });
  }

  // Read the queries from the file
  vector<string> queries;
  while (getline(input, line)) {
    queries.push_back(line);
  }

  // Search the phonebook for each query and write the matching entries to the output stream
  for (const auto &query : queries) {
    for (const auto &entry : phonebook) {
      if (entry.firstName == query || entry.lastName == query) {
        out << entry.firstName << " " << entry.lastName << " " << entry.phone << endl;
      }
    }
  }

  return true;
}

#ifndef __PROGTEST__
int main ()
{
  ostringstream oss;
  oss . str ( "" );
  assert ( report( "tests/test0_in.txt", oss ) == true );
  assert ( oss . str () ==
    "John Christescu 258452362\n"
    "John Harmson 861647702\n"
    //"-> 2\n"
    //"-> 0\n"
    "Josh Dakhov 264112084\n"
    "Dakhov Speechley 865216101\n"
    //"-> 2\n"
    "John Harmson 861647702\n");
    //"-> 1\n" );
  oss . str ( "" );
  assert ( report( "tests/test1_in.txt", oss ) == false );
  return 0;
}
#endif /* __PROGTEST__ */
