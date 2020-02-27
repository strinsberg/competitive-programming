///////////////////////////////////////////////////////////////////////
// CPSC 3200
// Assignment 4
// UVA 296
//
// Steven Deutekom
// Jan 13 2020
//
// For each test case we see how many of the codes from 0000 ... 9999
// would actually be possible with the given guesses. If there is only
// one it is output, more than 1 "indeterminate", none "impossible.
//
// I am sure there are easier ways to do some of the things I did, but
// I beleive the overall approach is sound. But this would take too
// long in a contest I think for a pretty easy problem.
////////////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;

// A struct to hold the info from a guess
struct Guess {
  string guess;
  int bulls;  // right pos, right num
  int cows;   // wrong pos, right num
};

// All the possible codes from 0000 ... 9999
vector<string> all_codes;

// Generates all codes
void gen_codes() {                                                             
  for (int i = 0; i < 10000; i++) {                                            
    string code = to_string(i);                                                
    while (code.length() < 4)                                                  
      code.insert(0, "0");                                                     
    all_codes.push_back(code);
  }                                                                            
}  

// Counts the number of bulls.
// Replaces them with * so they are not counted when checking for cows.
int num_bulls(string& code, string& guess) {
  int ans = 0;
  for (int i = 0; i < 4; i++) {
    if (code[i] == guess[i]) {
      ans++;
      code[i] = '*';
      guess[i] = '*';
    }
  }
  return ans;
}

// Counts the number of cows.
// Any time two nums are matched they are replaced with * so they won't
// be counted twice.
int num_cows(string& code, string& guess) {
  int ans = 0;
  for (auto& c : code) {
    if (c == '*') continue;
    for (auto& g : guess) {
      if (g != '*' and g == c) {
        ans++;
        c = g = '*';
      }
    }
  }
  return ans;
}

// Checks to see if a given code could be possible given the
// guess information
bool possible(string code, string guess, int bulls, int cows) {
  if (bulls != num_bulls(code, guess))
    return false;

  if (cows == num_cows(code, guess))
    return true;
  return false;
}

// Test all the codes to see if any will work with for all guesses.
string solve(vector<Guess>& guesses) {
  string ans = "";
  for (auto& code : all_codes) {
    bool p = true;
    for (auto& g : guesses) {
      if (!possible(code, g.guess, g.bulls, g.cows)) {
        p = false;
      }
    }

    // If the code works for every guess
    if (p) {
      if (ans == "") {
        ans = code;
      } else {
        ans = "indeterminate";
        break;
      }
    }
  }

  if (ans == "")
    ans = "impossible";

  return ans;
}

// Solve all the tests
int main() {
  // Precompute all the possible codes from 0000 ... 9999 so we only
  // do it once at the start.
  gen_codes();

  int t;
  cin >> t;

  for (int k = 0; k < t; k++) {
    int g;
    cin >> g;

    // Get all the guess information
    vector<Guess> guesses;
    for (int j = 0; j < g; j++) {
      Guess g;
      char _;
      cin >> g.guess;
      cin >> g.bulls >> _ >> g.cows;
      guesses.push_back(g);
    }

    cout << solve(guesses) << endl;
  }
  return 0;
}
