///////////////////////////////////////////////////////////////////////
// CPSC 3200
// Project
// rmc18 Problem I -> Mr. Plow King
//
// Steven Deutekom
// March 2 2020
//
// The plow will always plow the minimum spanning tree. So we need to
// figure out how edges will be placed so that we force the mst to be
// as big as possible. After placing an edge we can connect some edges
// from the newly connected node back to already connected nodes. As these
// nodes will create cycles they will not be in the mst. For the ith edge
// in the mst we can cause the next i - 1 edges to be ignored. this creates
// the sequence 1,2,4,7,11,16,....
//
// So we can greedily place edges in this pattern until we need all the
// remaining edges to finish the mst. The approach below builds the
// sequence and adds edges from it until doing so would not allow us
// to finish the mst. Then it adds the remaining edges that have not been
// placed yet to the mst. This is all done implicitly as we do not build
// a graph but just add the edge weight to the ans.
///////////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;

#define ll long long


int main() {
  // Build the sequence of edges our method will count if possible
  vector<ll> S;
  for (ll i = 1, j = 1; i < 1e12; i+=j, j++) {
    S.push_back(i);
  }

  // Get input
  ll n, m, left = 0;
  cin >> n >> m;

  // Add edges from S until we can no longer skip any edges
  ll ans = 0;
  for (int i = 0; i < (int)S.size(); i++) {
    if (m - S[i] < n - i - 1 ) {
      left = n - i - 1;  // The number of edges that still need to be placed
      break;
    }
    ans += S[i];
  }

  // Add all the remaining edges
  for (ll i = m; i > m - left; i--)
    ans += i;
  
  cout << ans << endl;

  return 0;
}
