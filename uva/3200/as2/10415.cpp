///////////////////////////////////////////////////////////////////////
// CPSC 3200
// Assignment 2
// UVA 10415
//
// Steven Deutekom
// Jan 9 2020
//
// Maps each note to the keys that must be down for it.
// For each note we check to see what keys will need to be pressed and increment
// counts in a vector indexed by key number.
///////////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;

// Map to keep track of key
map<char, set<int>> keys;

// Count all key presses for a string of notes keeping track in pressed
void solve(string notes, vector<int>& pressed) {
  set<int> last;
  for (auto c : notes) {
    for (auto& ks : keys[c]) {
      if (!last.count(ks))
        pressed[ks]++;
    }
    last = keys[c];
  }
}

int main() {
  // Map each note to a set of keys to press
  keys['c'] = set<int>{2,3,4,7,8,9,10};
  keys['d'] = set<int>{2,3,4,7,8,9};
  keys['e'] = set<int>{2,3,4,7,8};
  keys['f'] = set<int>{2,3,4,7};
  keys['g'] = set<int>{2,3,4};
  keys['a'] = set<int>{2,3};
  keys['b'] = set<int>{2};
  keys['C'] = set<int>{3};
  keys['D'] = set<int>{1,2,3,4,7,8,9};
  keys['E'] = set<int>{1,2,3,4,7,8};
  keys['F'] = set<int>{1,2,3,4,7};
  keys['G'] = set<int>{1,2,3,4};
  keys['A'] = set<int>{1,2,3};
  keys['B'] = set<int>{1,2};

  // Get input
  int n;
  cin >> n;
  cin.ignore();
  
  for (int i = 0; i < n; i++) {
    vector<int> pressed(11, 0);
    string notes;
    getline(cin, notes); // Use getline to properly deal with empty songs

    solve(notes, pressed);

    // Display the pressed button counts
    for (int j = 1; j < 11; j++) {
      cout << pressed[j];
      if (j < 10)
        cout << " ";
    }
     cout << endl;
  }

  return 0;
}
