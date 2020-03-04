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
// Graph we will have a node for each of the possible 128 color combinations.
// The colors that have been picked up are a kept in a bit mask that is used
// to index the graph along with the location one is at (Josip's idea).
// Each edge from a position will extend from these nodes to all the nodes
// at another position. This works because there are only 343 total positions
// possible in the race. Then we run a simple dijkstra on this graph from the
// start position with no colors gained yet G[0][0]. I wrote the dijkstra
// myself to accomadate the graph structure I am using. It's just a bfs with
// a priority queue and a different restriction for adding neighbors to the
// queue.
///////////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;

// Some defines to reduce typing with 3 dimensional arrays and pairs of pairs
#define vec vector
#define sec second
#define fst first
#define pii pair<int, int>
#define pip pair<int, pii>
#define ll long long
#define INF 1e18


int N,M;
vec<vec<vec<pip>>> G;  // Graph that is G[node][colorsGained][(neighbor, dist, color)]
vec<vec<ll>> dist;  // Distance to [node][colorsGained] from start


// Map from each color char to the number that has that bit turned on
// for the bitmasks for the colors
map<char, int> col{
  {'R', 1}, {'O', 2}, {'Y', 4}, {'G', 8}, {'B', 16}, {'I', 32}, {'V', 64}
};


// A simple dijkstra to solve the problem
// start is [0][0] and end is [0][127]
ll solve() {
  dist = vec<vec<ll>>(N, vec<ll>(128, INF));
  priority_queue<pii, vec<pii>, greater<pii>> q;

  q.emplace(0, 0);
  dist[0][0] = 0;

  while (!q.empty()) {
    pii v = q.top(); q.pop();

    for (auto& u : G[v.first][v.second]) {
      int colors = v.sec | u.sec.sec;  // Pick up the color

      if (dist[v.fst][v.sec] + u.sec.fst < dist[u.fst][colors]) {
        dist[u.fst][colors] = dist[v.fst][v.sec] + u.sec.fst;  // Add to the distance
        q.emplace(u.fst, colors);
      }
    }
  }

  return dist[0][127];
}


int main() {
  cin >> N >> M;

  // Build the graph with N nodes that each have the 128 different color
  // combinations
  G = vec<vec<vec<pip>>>(N, vec<vec<pip>>(128));
  for (int i = 0; i < M; i++) {
    int a, b, d; cin >> a >> b >> d;
    char c; cin >> c;
    a--; b--;

    for (int j = 0; j < 128; j++) {
      G[a][j].push_back({b, {d, col[c]}});
      G[b][j].push_back({a, {d, col[c]}});
    }
  }

  cout << solve() << endl;

  return 0;
}
