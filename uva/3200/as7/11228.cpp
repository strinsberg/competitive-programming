///////////////////////////////////////////////////////////////////////
// CPSC 3200
// Assignment 7
// UVA 11228
//
// Steven Deutekom
// March 2 2020
//
// Use kruskals alg to find the minimum spanning tree of the cities.
// Then for each edge in the mst we just find if it is a rail or a road by
// comparing it to r. Also, the number of states is the number of rails
// needed + 1.
// ////////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;


// Copy pasted library code. Only slightly modified ;) ///////////////////////

class UnionFind
{
  struct UF { int p; int rank; };

 public:
  UnionFind(int n) {          // constructor
    howMany = n;
    uf = new UF[howMany];
    for (int i = 0; i < howMany; i++) {
      uf[i].p = i;
      uf[i].rank = 0;
    }
  }

  ~UnionFind() {
     delete[] uf;
  }

  int find(int x) { return find(uf,x); }        // for client use
      
  bool merge(int x, int y) {
    int res1, res2;
    res1 = find(uf, x);
    res2 = find(uf, y);
    if (res1 != res2) {
      if (uf[res1].rank > uf[res2].rank) {
        uf[res2].p = res1;
      }
      else {
        uf[res1].p = res2;
        if (uf[res1].rank == uf[res2].rank) {
          uf[res2].rank++;
        }
      }
      return true;
    }
    return false;
  }
      
 private:
  int howMany;
  UF* uf;

  int find(UF uf[], int x) {             // for internal use
    if (uf[x].p != x) {
      uf[x].p = find(uf, uf[x].p);
    }
    return uf[x].p;
  }
};

class Edge {

 public:
  Edge(int i=-1, int j=-1, double weight=0) {
    v1 = i;
    v2 = j;
    w = weight;
  }
  bool operator<(const Edge& e) const { return w < e.w; }

  int v1, v2;          /* two endpoints of edge                */
  double w;            /* weight, can be double instead of int */
};


// Slightly modified to take vectors instead of arrays
double mst(int n, int m, vector<Edge>& elist, vector<int>& index, int& size)
{
  UnionFind uf(n);

  sort(elist.begin(), elist.end());

  double w = 0.0;
  size = 0;
  for (int i = 0; i < m && size < n-1; i++) {
    int c1 = uf.find(elist[i].v1);
    int c2 = uf.find(elist[i].v2);
    if (c1 != c2) {
      index[size++] = i;
      w += elist[i].w;
      uf.merge(c1, c2);
    }
  }

  return w;
}


// My Code ///////////////////////////////////////////////////////////////////

void solve(vector<Edge> E, int n, double r, int& c) {
  vector<int> I(n);
  int T, states = 1;
  double roads = 0.0, rails = 0.0;

  // Calculate min span tree
  mst(n, (int)E.size(), E, I, T);

  // For each edge in the mst if greater than r it is a rail
  // otherwise it is a road
  // number of states is rails + 1
  for (int k = 0; k < T; k++) {
    int i = I[k];
    if (E[i].w > (double)r) {
      rails += E[i].w;
      states++;
    } else {
      roads += E[i].w;
    }
  }

  // Print the answer
  cout << "Case #" << c++ << ": " << states << " " << round(roads) << " " << round(rails) << endl;
}


int main() {
  int c = 1;
  int t; cin >> t;

  while (t--) {
    // Get input
    int n, r;
    cin >> n >> r;

    vector<pair<int,int>> C(n);
    for (auto& x : C) cin >> x.first >> x.second; 

    // Connect each city to every other city
    vector<Edge> E;
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (i!=j) E.emplace_back(i, j, hypot(C[i].first - C[j].first, C[i].second - C[j].second));
      }
    }

    // Solve the test case and print answer
    solve(E, n, r, c);
  }

  return 0;
}
