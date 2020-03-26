///////////////////////////////////////////////////////////////////////
// CPSC 3200
// Assignment 8
// UVA 384
//
// Steven Deutekom
// March 25 2020
//
// Solution as described in video. Uses function to identify each type of
// possible word. Just did the simpler method of taking substrings and
// recursing where needed.
///////////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'


bool is_slump(string str) {
  // Check size and first char
  if (str.size() == 0 or (str[0] != 'D' and str[0] != 'E'))
    return false;
  
  // Check next char is F and skip all extra Fs
  if (str[1] != 'F')
    return false;

  int i = 2;
  while (str[i] == 'F') i++;

  // If next char is a G and it is the last char return true
  // Otherwise recurse on the rest to see if it is a slump
  if (str[i] == 'G' and i == (int)str.size() - 1)
    return true;
  else
    return is_slump(str.substr(i, str.size() - i));
}


bool is_slimp(string str) {
  // Check size and make sure first char is an A
  if (str.size() == 0 or str[0] != 'A')
    return false;
  else if (str.size() < 2)
    return false;

  // If 2 char slimp check to make sure next char is H
  if (str.size() == 2)
    return str[1] == 'H';

  // If Check last rule with slimps and slumps
  // make sure last char is a C
  if ((str[1] == 'B' and is_slimp(str.substr(2, str.size() - 3)))
       or is_slump(str.substr(1, str.size() - 2)))
    return str[str.size() - 1] == 'C';
  else
    return false;
}


bool is_slurpy(string str) {
  // Check each way of splitting the strings into 2 peices
  // if one works then return true.
  for (int i = 0; i < (int)str.size(); i++) {
    string first = str.substr(0, i + 1);
    string second = str.substr(i + 1, str.size() - i);

    if (is_slimp(first) and is_slump(second))
      return true;
  }
  return false;
}


int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int n; cin >> n;

  cout << "SLURPYS OUTPUT" << endl;

  // For each case print wether it is a slurpy or not
  while (n--) {
    string str; cin >> str;
    if (is_slurpy(str))
      cout << "YES" << endl;
    else
      cout << "NO" << endl; 
  }

  cout << "END OF OUTPUT" << endl;

  return 0;
}
