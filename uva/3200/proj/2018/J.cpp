///////////////////////////////////////////////////////////////////////
// CPSC 3200
// Project
// rmc18 Problem j -> Rainbow Road Race
//
// Steven Deutekom
// March 2 2020
//
// In order to find the paths containing all the colors we can do
// something similar to the 'color wheel' problem. For each position on the
// Graph we will have a state for each group of colors we may have when
// we are there. Since there are 7 colors this adds 2^7 nodes for each
// one of the positions. This will give us 343 * 128 = 43904 nodes.
//
// To find the shortest path we can use dijkstra and travel from the
// 0 node with no colors to the 0 node with all colors.
///////////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define INF 1e18

int N,M;
vector<vector<pair<int, pair<int, int>> G;
vector<vector<ll>> dist;

ll dijkstra() {
  dist = vector<vector<int>>(N, vector<int>(128, INF));
  q = priority_queue<pair<int, int>>, vector<pair<int, int>>, greater<pair<int, int>>>();

  q.emplace(0, 0);
  dist[0][0] = 0;

  while (!q.empty()) {
    pair<int, int> v = q.front(); q.pop();

    for (auto& u : G[v.first][v.second]) {
      int colors = v.second | u.second.second;  // Pick up a color
      if (dist[v.first][v.second] + u.second < dist[u.first][colors]) {
        dist[u.first][colors] = dist[v.first][v.second] + u.second.first;  // Add to the distance
        q.emplace(u.first, colors);
      }
    }
  }
}


int main() {
  cin >> N >> M;

  G = vector<vector<pair<int, pair<int, int>>>>(N);
  for (int i = 0; i < M; i++) {
    int a, b, d; cin >> a >> b >> d;
    char c; cin >> c;

    G[a].push_back({b, {d, col[c]}};
    G[b].push_back({a, {d, col[c]}};
  }

  cout << dijkstra() << endl;

  return 0;
}
