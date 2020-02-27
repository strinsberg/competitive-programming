///////////////////////////////////////////////////////////////////////
// CPSC 3200
// Assignment 2
// UVA 10189
//
// Steven Deutekom
// Jan 8 2020
//
// Records locations of each mine and then for each mine update each space
// touching the mine by 1.
///////////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;

// Updates the spaces around the 
void update(int x, int y, vector<vector<int>>& M) {
  for (auto i : {-1, 0, 1}) {
    for (auto j : {-1, 0, 1}) {
      if (i==0 && j==0) continue;

      // mines are stored as -1 so do not increment mines
      if (M[x + i][y + j] >= 0)
        M[x + i][y + j]++;
    }
  }
}

// Main function solves the problem ;)
int main() {
  int m, n, field = 1;
  while (cin >> m >> n && m) {
    // to only print new lines **between** cases
    if (field > 1)
      cout << endl;

    // Read the input. Replace '*' with -1.
    vector<vector<int>> M(m + 2, vector<int>(n + 2, 0));
    vector<pair<int, int>> mines;
    for (int i = 1; i <= m; i++) {
      for (int j = 1; j <= n; j++) {
        char temp;
        cin >> temp;
        if (temp == '*') {
          M[i][j] = -1;
          mines.emplace_back(i,j);
        }
      }
    }

    // Update the values around each mine
    for (auto i : mines)
      update(i.first, i.second, M);

    // Print out the field. Replace -1 with '*'.
    cout << "Field #" << field++ << ":" << endl;
    for (int i = 1; i <= m; i++) {
      for (int j = 1; j <= n; j++) {
        int x = M[i][j];
        if (x < 0)
          cout << '*';
        else
          cout << x;
      }
      cout << endl;
    }
  }
  return 0;
}
