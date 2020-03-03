///////////////////////////////////////////////////////////////////////
// CPSC 3200
// Assignment 7
// UVA 10004
//
// Steven Deutekom
// March 2 2020
//
// Build the graph and start at node 0 with either color. Then dfs the graph.
// If we find an colored neighbor it must be the opposite color of us or it
// is not possible to bi-color it. If the neighbor is not colored we dfs it
// and if any of those neighbors returns false it is not bi-colorable.
///////////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;


// color edges. If there is colored edge already visited return false if
// it is the opposite.
bool dfs(int v, int c, vector<vector<pair<int,int>>>& G) {
  bool ans = true;

  for (auto& it : G[v]) {
    if (it.second == -1)
      it.second = c ^ 1, ans &= dfs(it.first, c ^ 1, G);
    else if (it.second == c)
      return false;
  }

  return ans;
}


int main() {
  int n,l;

  while(cin >> n and n) {
    cin >> l;
    vector<vector<pair<int,int>>> G(n);

    // Build the graph
    for (int i = 0; i < l; i++) {
      int a,b;
      cin >> a >> b;
      G[a].emplace_back(b, -1);
      G[b].emplace_back(a, -1);
    }
 
    // Print the answer
    if (dfs(0, 0, G))
      cout << "BICOLORABLE." << endl;
    else
      cout << "NOT BICOLORABLE." << endl;
  }

  return 0;
}
