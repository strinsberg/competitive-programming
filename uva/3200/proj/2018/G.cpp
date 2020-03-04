///////////////////////////////////////////////////////////////////////
// CPSC 3200
// Project
// rmc18 Problem G -> Neighborhood Watch
//
// Steven Deutekom
// March 2 2020
//
// The number of paths between 2 safe houses is the number of the first
// house times the number of bad houses plus 1 for the first safe house.
// So for each house we can add the number of safe paths to the total.
// We just need to make sure we add an implicit safe house past the end
// to properly count any bad houses that might not be followed by a safe
// house.
///////////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;

#define ll long long


int main() {
  int n, k;
  cin >> n >> k;

  vector<ll> A(k);
  for (auto& x : A) cin >> x;
  A.push_back(n + 1);

  ll ans = 0;
  for (int i = 0; i < k; i++)
    ans += A[i] * (A[i + 1] - A[i]);
    
  cout << ans << endl;
    
  return 0;
}
