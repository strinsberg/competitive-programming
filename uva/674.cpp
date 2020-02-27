////////////////////////////////////////////////////////////////
// CPSC 3200
// Assignment 6
// UVA 647
//
// Steven Deutekom
// Feb 22 2020
//
// Build a dp table where each row is the number of ways to make
// n cents change using up to the current coin denomination. This
// is calculated by finding the ways to make the change without
// using any of the current coin + the number of ways to make it
// using 1 or more of the current coins.
////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'


int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  vector<vector<ll>> dp(5, vector<ll>(10000, 1));
  vector<int> coins{1,5,10,25,50};

  // Build a dp table skips row 0 as there is always only 1 way
  for (int i = 1; i < 5; i++) {
    for (int j = 5; j < 10000; j++) {
      dp[i][j] = dp[i-1][j];

      int amt = j - coins[i];
      if (amt >= 0) {
        dp[i][j] += dp[i][amt];
      }
    }
  }

  // Get each amount to make change for and print the result
  int n;
  while (cin >> n) { 
    cout << dp[4][n] << endl;
  }

  return 0;
}
