///////////////////////////////////////////////////////////////////////
// CPSC 3200
// Assignment 1
// UVA 305
//
// Steven Deutekom
// Jan 7 2020
//
// Solves the problem by trying all m for a given k.
//
// This only works because k < 14 and we are not actually maintaining
// any data structure when trying to determine which entities are
// being killed each round.
// So there are at most 13 things to check for a given m.
//
// Also, saves computed answers so it is fast if they are asked for
// again. So a test with all 13's will be as fast as doing it once.
///////////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;

// Check if a given k and m will kill all bad guys before any good guys.
bool win(int k, int m) {
  int n = 2 * k;
  int i = 0;
  for (int j = 0; j < k; j++) {
    i = (i + m - 1) % (n - j);
    if (i < k)
      return false;
  }
  return true;
}

// For a given k see if any m will win.
// Return the smallest winning m.
int solve(int k) {
  for (int m = k + 1; m < 1e9; m++) {
    if (win(k, m))
      return m;
  }
  return -1;  // Should never happen in our cases. Debug.
}

// DP table so that if they ask for the same value more than one time it
// is fast to give the answer. This was necessary for me to avoid TLE.
vector<int> table(13 + 1, -1);

// Get input and solve the problem
int main() {
  while (1) {
    int k;
    cin >> k;

    if (k == 0) break;

    // Only calculate it if we have not done so yet
    if (table[k] == -1)
      table[k] = solve(k);

    cout << table[k] << endl;
  }

  return 0;
}
