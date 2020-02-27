///////////////////////////////////////////////////////////////////////
// CPSC 3200
// Assignment 1
// UVA 409
//
// Steven Deutekom
// Jan 7 2020
//
// For each excuse we find each word that has only alpha characters in
// it we check if that is in the set of keywords. For each occurence
// we keep track of it and output all excuses that had the max score.
//
// I was doing this backwards by trying to count all occurences of each
// keyword in the excuse to get the lame score. **Josip** set me
// straight ;)
///////////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;

// Returns a copy of the string in all lowercase
string to_lower(string str) {
  for (int i = 0; i < str.size(); i++)
    str[i] = tolower(str[i]);
  return str;
}

// Find the number of occurences of keywords in a string
int occurences(string exc, set<string>& keys) {
  int ans = 0;
  string str = to_lower(exc);
  string word;

  // Loop through characters. If we have an alpha char add it to
  // a word. If it is not alpha then check if the word is a key word and
  // increment the number of occurences accordingly and reset the word.
  for (auto& c : str) {
    if (isalpha(c)) {
      word += c;
    } else {
      ans += keys.count(word);
      word.clear();
    }
  }
  ans += keys.count(word);

  return ans;
}

// Solve the problem
int main() {
  int j = 1, n, m;

  while (cin >> n >> m && cin) {

    // Put the keywords into the set
    set<string> keys;
    for (int i = 0; i < n; i++) {
      string temp;
      cin >> temp;
      keys.insert(temp);
    }

    // Find the number of occurences in each string
    // and keep track of them with their score.
    int worst = 0;
    map<int, set<string>> lame;
    cin.ignore();
    
    for (int i = 0; i < m; i++) {
      string excuse;
      getline(cin, excuse);
      int score = occurences(excuse, keys);
      lame[score].insert(excuse);
      worst = max(score, worst); 
    }
  
    // Print out the info for the case.
    cout << "Excuse Set #" << j++ << endl;
    for (auto& s : lame[worst])
      cout << s << endl;
    cout << endl;
  }

  return 0;
}
