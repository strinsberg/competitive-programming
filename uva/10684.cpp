////////////////////////////////////////////////////////////////
// CPSC 3200
// Assignment 6
// UVA 10684
//
// Steven Deutekom
// Feb 22 2020
//
// Maximum increase is the largest difference between the current sum
// and the lowest point the sum has been at so far. If no bet is ever
// greater than 0 there is no possibility of a winning streak.
////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'


int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int n;
  while (cin >> n and n) {
    bool win = false;
    ll sum = 0, mn = 0, streak = 0;

    // Read input and calculate the biggest streak
    for (int i = 0; i < n; i++) {
      ll x; cin >> x;
      sum += x;
      if (x > 0) win = true;

      if (sum < mn)
        mn = sum;
      else
        streak = max(streak, sum - mn);
    }

    // Display the answer
    if (!win) {
      cout << "Losing streak." << endl;
    } else {
      cout << "The maximum winning streak is " << streak << "." << endl;
    }
  }

  return 0;
}
