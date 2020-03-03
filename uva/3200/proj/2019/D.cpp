///////////////////////////////////////////////////////////////////////
// CPSC 3200
// Project
// rmc19 Problem D -> Integer Division
//
// Steven Deutekom
// March 2 2020
//
// Divide each number by the divisor and save how many of the results are
// the same. Then sum up all the divisions. Use long long so you don't get
// wrong answer like we all did in the contest.
///////////////////////////////////////////////////////////////////////
#include<bits/stdc++.h>
using namespace std;

#define ll long long


int main() {
  ll n, d, ans = 0;
  cin >> n >> d;

  // Count all the divisions for each number by d
  map<ll, ll> M;
  for(int i = 0; i < n; i++) {
    ll x; cin>>x;
    M[x/d]++;
  }

  // Get the total sum of all the possible divisions
  for(auto& it : M)
    ans += (it.second * (it.second - 1)) / 2;

  cout << ans << endl;
}
