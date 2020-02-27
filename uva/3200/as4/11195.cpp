///////////////////////////////////////////////////////////////////////
// CPSC 3200
// Assignment 4
// UVA 11195
//
// Steven Deutekom
// Jan 20 2020
//
// Use recursion and backtracking to sovle the problem. Also, I implemented
// The boolean lookups for checking if a row is occupied, as demonstrated in
// class.
//
// I had some time limit issues even though I beleive the method is
// essentially correct. I got accepted by using some io tricks and
// vector<int> instead of vector<bool>. However, I was able to get an extra
// 2 seconds faster by restructuring my recursion to place the queen and
// then recursing only when a queen could be placed in a column in the
// next row.
///////////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;

#define endl '\n'

// Declare variables global
int n;
vector<vector<int>> blocked;
vector<int> diag;
vector<int> rdiag;
vector<int> col;

// Place a queen and see how many solutions can be obtained
int place(int r, int c) {
  // If a queen can't be placed. Only really matters for r=0.
  if (blocked[r][c]) return 0;

  // Place the queen
  col[c] = 1;
  diag[r+c] = 1;
  rdiag[r-c+n] = 1;

  // For each column in the next row, if a queen can be placed get the
  // number of solutions.
  r++;
  int ans = 0;
  for (int i = 0; i < n; i++) {
    bool bad = (col[i] || diag[r+i] || rdiag[r-i+n] || blocked[r][i]);

    if (r == n - 1)
      ans += !bad;
    else if (!bad)
      ans += place(r,i);
  }
  r--;

  // Remove the queen when backtracking
  col[c] = 0 ;
  diag[r+c] = 0;
  rdiag[r-c+n] = 0;

  return ans;
}

int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int c = 1;
  while (cin >> n && n) {
    // Get all the blocked squares
    blocked = vector<vector<int>>(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        char c;
        cin >> c;
        blocked[i][j] = c == '*' ? 1 : 0;
      }
    }

    // Initialize the boolean arrays
    col = vector<int>(n);
    diag = vector<int>(n+n);
    rdiag = vector<int>(n+n);

    // Find all successful solutions for placing a queen in row 0
    int ans = 0;
    for (int c = 0; c < n; c++)
      ans += place(0, c);

    cout << "Case " << c++ << ": " << ans << endl;
  }

  return 0;
}
