///////////////////////////////////////////////////////////////////////
// CPSC 3200
// Assignment 1
// UVA 409
//
// Steven Deutekom
// Jan 7 2020
//
// For each excuse replace all punctiation with spaces and use the string
// stream as suggested in class. Then count how many words in the escuse
// are in the set of keywords.
//
// This is my second solution to make you happy!
///////////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;

// Fix a string by removing all punctuation and lowercasing all letters
// copies the string.
string fix(string str) {
  for (int i = 0; i < str.size(); i++) {
    if (isalpha(str[i]))
      str[i] = tolower(str[i]);
    else
      str[i] = ' ';
  }
  return str;
}

// Main solves the problem ;)
int main() {
  int c = 1, n, m;

  while (cin >> n >> m) {
    if (c > 1)
      cout << endl;

    // Get all keys
    set<string> keys;
    for (int i = 0; i < n; i++) {
      string temp;
      cin >> temp;
      keys.insert(temp);
    }

    // Get all excuses
    cin.ignore();
    vector<string> exc(m);
    for (int i = 0; i < m; i++) {
      string line;
      getline(cin, line);
      exc.push_back(line);
    }

    // For each excuse get the fixed version of the string and then
    // Count how many keywords are in it.
    // Keep track of the max number.
    map<int, set<string>> freq;
    int worst = 0;
    for (auto& e : exc) {
      int score = 0;
      stringstream ss(fix(e));
      string s;
      while(ss >> s) {
        if (keys.count(s))
          score++;
      }
      worst = max(score, worst);
      freq[score].insert(e);
    }

    // Print out the test case results.
    cout << "Excuse Set #" << c++ << endl;
    for (auto& s : freq[worst])
      cout << s << endl;
  }
  return 0;
}
