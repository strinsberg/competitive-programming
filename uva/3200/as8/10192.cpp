///////////////////////////////////////////////////////////////////////
// CPSC 3200
// Assignment 8
// UVA 10192
//
// Steven Deutekom
// March 25 2020
//
// Solution is the recursive dp solution explained in the video.
///////////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'

string S,T;
vector<vector<int>> DP;


// Recursive solution with DP lookup as explained in video
int solve(int i, int j) {
  if (DP[i][j] != -1)
    return DP[i][j];

  if (!i or !j) {
    DP[i][j] = 0;
    return 0;
  }
  
  if (S[i-1] == T[j-1])
    DP[i][j] = solve(i-1, j-1) + 1;
  else
    DP[i][j] = max(solve(i-1, j), solve(i, j-1));

  return DP[i][j];
}


int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int c = 1;
  while (1) {
    // Get input and create dp table
    getline(cin, S); if (S[0] == '#') break;
    getline(cin, T);
    DP = vector<vector<int>>(101, vector<int>(101, -1));

    // Solve and output result
    cout << "Case #" << c++ << ": ";
    cout << "you can visit at most " << solve(S.size(), T.size());
    cout << " cities." << endl;
  }
  return 0;
}
