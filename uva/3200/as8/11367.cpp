///////////////////////////////////////////////////////////////////////
// CPSC 3200
// Assignment 8
// UVA 11367
//
// Steven Deutekom
// March 24 2020
//
// Approach is that described in the video. Except that I used Graph
// that is a 2d adjacency list of "edges". An edge is 3 items: the
// location, the fuel level, and the cost of the edge.
//
// I wrote my own quick dijkstra for this to accomadate the different
// Graph structure. It would have worked fine if I hadn't pushed
// nodes into the priority queue without the total distance traveled ;)
///////////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;

// Make some things simpler
#define INF 1e9
#define ll long long
#define endl '\n'
#define pii pair<int,int>
#define ppi pair<int,pii>
#define vec vector
#define fst first
#define sec second

vec<vec<vec<ppi>>> G(1010, vec<vec<ppi>>(101));  // Graph[location][feul]
vec<vec<int>> D,V;  // Distance and Visited Arrays


// A dijkstra to solve the problem
int solve(int s, int e, int c) {
  D = vec<vec<int>>(1010, vec<int>(101, INF));
  V = vec<vec<int>>(1010, vec<int>(101, 0));
  priority_queue<ppi, vec<ppi>, greater<ppi>> q;

  // fst is Dist, sec is location, and third is fuel level
  q.push({0, {s, 0}});  
  D[s][0] = 0;

  while (!q.empty()) {
    ppi v = q.top(); q.pop();

    // Don't reprocess a node
    if (V[v.sec.fst][v.sec.sec]) continue;
    V[v.sec.fst][v.sec.sec] = 1;

    // Add all edges we can travel on
    for (auto& u : G[v.sec.fst][v.sec.sec]) {
      int w = D[v.sec.fst][v.sec.sec] + u.sec.sec;

      if (u.sec.fst <= c and w < D[u.fst][u.sec.fst]) {
        D[u.fst][u.sec.fst] = w;
        q.push({w, {u.fst, u.sec.fst}}); 
      }
    }
  }

  return D[e][0];
}


// Get the graph and solve all the queries
int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);
  int n, m; cin >> n >> m;

  // Add feul up edges
  for (int i = 0; i < n; i++) {
    int p; cin >> p;
    for (int j = 0; j < 100; j++)
      G[i][j].push_back({i, {j+1, p}});
  }

  // Add feul loss edges
  for (int i = 0; i < m; i++) {
    int u, v, d; cin >> u >> v >> d;
    for (int j = 100; j-d >= 0; j--) {
      G[u][j].push_back({v,{j-d, 0}});
      G[v][j].push_back({u,{j-d, 0}});
    }
  }
 
  // solve each query 
  int q; cin >> q;
  for (int i = 0; i < q; i++) {
    int c,s,e; cin >> c >> s >> e;

    int ans = solve(s,e,c);
    if (ans == INF)
      cout << "impossible" << endl;
    else
      cout << ans << endl;
  }

  return 0;
}
