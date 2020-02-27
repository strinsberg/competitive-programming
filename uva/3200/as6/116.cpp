////////////////////////////////////////////////////////////////
// CPSC 3200
// Assignment 6
// UVA 116
//
// Steven Deutekom
// Feb 23 2020
//
// Starting at the end we work backwards storing the best path
// cost of a cell + it's possible destinations. Store a pointer
// to the next cell in the path in another graph P.
//
// At the end we can find the index of the smallest path cost in
// the first column. If there are ties we always keep the minimum
// index. Then starting here we reconstruct the path by moving
// through P.
/////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;

#define ll long long


vector<vector<ll>> G, P;
int n,m;


int solve() {
  // Compute path values and store path
  for (int j = n - 1; j >= 0; j--) {
    for (int i = 0; i < m; i++) {
      ll a = (i+1) % m, b = (i+m-1) % m;
      vector<pair<ll,ll>> M = {{G[i][j+1], i}, {G[a][j+1], a}, {G[b][j+1], b}};
      sort(M.begin(), M.end());

      G[i][j] += M[0].first;
      P[i][j] = M[0].second;
    }
  }

  // Find best path
  ll p = 0;
  for (int i = 1; i < m; i++) {
    if (G[i][0] < G[p][0]) {
      p = i;
    }
  }

  return p;
}


int main() {
  while (cin >> m >> n) {
    G = vector<vector<ll>>(m, vector<ll>(n+1, 0));
    P = vector<vector<ll>>(m, vector<ll>(n+1, 0));

    // Get input
    for (int i = 0; i < m; i++) {
      for (int j = 0; j < n; j++) {
        cin >> G[i][j];
      }
    }

    int p = solve();

    // print the path
    int ans = G[p][0];
    for (int i = 0; i < n; i++) {
      cout << p + 1;
      p = P[p][i];
      if (i < n - 1) cout << " ";
    }
    cout << endl;

    // print the path weight
    cout << ans << endl;
  }
  return 0;
}
