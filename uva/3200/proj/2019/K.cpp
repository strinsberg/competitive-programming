///////////////////////////////////////////////////////////////////////
// CPSC 3200
// Project
// rmc19 Problem K -> Lost Lineup
//
// Steven Deutekom
// March 2 2020
//
// Create a list of people in the proper order. Their new index is Jimmy's
// plus the number of people between them.
///////////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;


int main() {
  int n, s;
  cin >> n;

  // For each person in the line (jimmy is first) place them in the line
  vector<int> A(n, 1);
  for(int i = 2; i <= n; i++) {
    cin >> s;
    A[s+1] = i;
  }

  // Print ans
  for(auto i : A)
    cout << i << " ";
  cout << endl;

  return 0;
}
