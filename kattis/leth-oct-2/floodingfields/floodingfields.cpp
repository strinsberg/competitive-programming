#include <bits/stdc++.h>
using namespace std;

struct Edge;
typedef list<Edge>::iterator EdgeIter;

struct Edge {
  int to;
  int cap;
  int flow;
  bool is_real;
  EdgeIter partner;
  
  Edge(int t, int c, bool real = true)
    : to(t), cap(c), flow(0), is_real(real)
  {};

  int residual() const
  {
    return cap - flow;
  }
};

struct Graph {
  list<Edge> *nbr;
  int num_nodes;
  Graph(int n)
    : num_nodes(n)
  {
    nbr = new list<Edge>[num_nodes];
  }

  ~Graph()
  {
    delete[] nbr;
  }

  // note: this routine adds an edge to the graph with the specified capacity,
  // as well as a residual edge.  There is no check on duplicate edge, so it
  // is possible to add multiple edges (and residual edges) between two
  // vertices
  void add_edge(int u, int v, int cap)
  {
    nbr[u].push_front(Edge(v, cap));
    nbr[v].push_front(Edge(u, 0, false));
    nbr[v].begin()->partner = nbr[u].begin();
    nbr[u].begin()->partner = nbr[v].begin();
  }
};

void push_path(Graph &G, int s, int t, const vector<EdgeIter> &path, int flow)
{
  for (int i = 0; s != t; i++) {
    if (path[i]->is_real) {
      path[i]->flow += flow;
      path[i]->partner->cap += flow;
    } else {
      path[i]->cap -= flow;
      path[i]->partner->flow -= flow;
    }
    s = path[i]->to;
  }
}

// the path is stored in a peculiar way for efficiency: path[i] is the
// i-th edge taken in the path.
int augmenting_path(const Graph &G, int s, int t, vector<EdgeIter> &path,
		    vector<bool> &visited, int step = 0)
{
  if (s == t) {
    return -1;
  }
  for (EdgeIter it = G.nbr[s].begin(); it != G.nbr[s].end(); ++it) {
    int v = it->to;
    if (it->residual() > 0 && !visited[v]) {
      path[step] = it;
      visited[v] = true;
      int flow = augmenting_path(G, v, t, path, visited, step+1);
      if (flow == -1) {
	return it->residual();
      } else if (flow > 0) {
	return min(flow, it->residual());
      }
    }
  }
  return 0;
}

// note that the graph is modified
int network_flow(Graph &G, int s, int t)
{
  vector<bool> visited(G.num_nodes);
  vector<EdgeIter> path(G.num_nodes);
  int flow = 0, f;

  do {
    fill(visited.begin(), visited.end(), false);
    if ((f = augmenting_path(G, s, t, path, visited)) > 0) {
      push_path(G, s, t, path, f);
      flow += f;
    }
  } while (f > 0);
  
  return flow;
}

/*
int main(void)
{
  Graph G(100);
  int s, t, u, v, cap, flow;

  cin >> s >> t;
  while (cin >> u >> v >> cap) {
    G.add_edge(u, v, cap);
  }
  
  flow = network_flow(G, s, t);
  cout << "maximum flow = " << flow << endl;
  
  return 0;
}
*/
// end flow
//

vector<int> delta{-1, 0, 1};

pair<int,int> nodes(int row, int col, int hour, int size) {
  int a = (row * 2 * size) + 2 + col;
  int b = a + size;
  return pair<int,int>{a, b};
}

void add_edges(const pair<int, int>& cow, int h,
               const vector<int>& flood, const vector<vector<int>>& heights,
               Graph& G) {
  cout << cow.first << " " << cow.second << " " << h << endl;
  auto node = nodes(cow.first, cow.second, h, heights.size());
  G.add_edge(node.first, node.second, 1);  // only 1 per square
  if (h == 2) {//flood.size()) {
    G.add_edge(node.second, 1, 1);
    return;
  }

  // add nodes to all next places
  for (auto dc : delta) {
    for (auto dr : delta) {
      if (abs(dc) == 1 && abs(dr) == 1) continue;
      // add nodes for the next position if it is a valid position
      int row = cow.first + dr;
      int col = cow.second + dc;
      cout << "  " << row << " " << col << " " << heights.size() << endl;
      if (row >= 0 && row < heights.size() && col >= 0 && col < heights.size()) {
        cout << "  " << flood[h] << " " << heights[row][col] << endl;
        if (heights[row][col] > flood[h]) {
          pair<int, int> new_cow{row, col};
          add_edges(cow, h + 1, flood, heights, G);
        }
      }
    }
  }
}


int main() {
  // get input
  int N, K, H;
  cin >> N >> K >> H;

  vector<vector<int>> heights(N, vector<int>(N));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      cin >> heights[i][j];
    }
  }

  vector<pair<int, int>> cows;
  for (int i = 0; i < K; i++) {
    int r, c;
    cin >> r >> c;
    cows.emplace_back(r,c);
  }

  vector<int> flood(H+1, 0);
  for (int i = 0; i < H; i++) {
    cin >> flood[i+1];
  }

  // create graph
  Graph g((H*N*N*2) + 2);
  for (auto& c : cows) {
    auto start = nodes(c.first, c.second, 0, N);
    g.add_edge(0, start.first, 1);
    add_edges(c, 0, flood, heights, g);
  }

  int flow = network_flow(g, 0, 1);
  cout << flow << endl;

  return 0;
}
