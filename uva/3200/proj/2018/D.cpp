///////////////////////////////////////////////////////////////////////
// CPSC 3200
// Project
// rmc18 Problem D -> H-Index
//
// Steven Deutekom
// March 2 2020
//
// Get all the numbers and sort them from largest to smallest.
// Then calculate the H-index according to it's formula, which I had to
// look on wikipedia up because I find this question incomprehensible.
///////////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;

int main() {
  int n, ans = 0;
  cin >> n;

  vector<int> A(n);
  for (auto& x : A) cin >> x;
  sort(A.begin(), A.end(), greater<int>());

  for (int i = 0; i < n; i++)
    ans = max(ans, min(A[i], i + 1));

  cout << ans << endl;

  return 0;
}
