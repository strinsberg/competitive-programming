///////////////////////////////////////////////////////////////////////
// CPSC 3200
// Assignment 7
// UVA 10047
//
// Steven Deutekom
// Feb 27 2020
//
// THIS is LONG and I did not put it all into functions, but I don't feel
// like trying to decompose it now, so I accept that you may take off some
// marks.
//
// Create a graph where each cell consists of nodes that keep track of
// direction and color. Connections include nodes within the same cell
// that represent changing direction and nodes in neighboring cells that
// keep the current direction but change the wheel color. In this graph
// each cell is 20 nodes, 5 for each direction. This approach may not be
// the best way to do this as each node is represented by a number that
// is not too hard to reason about, but is easy to make mistakes with. It is
// also very hard to debug, because when checking what path is traversed the
// nodes are printed with numbers and one must work out what nodes they
// should be connected to.
//
// Once the graph is built it is just a quick bfs. This can do a shortest path
// since all edge weights are 1. If we find a green node in the final cell
// (which we keep track of at the start) then we output the time it took to
// get there. In the bfs the nodes are pushed in a pair with the time to
// get there.
//
// It was worth doing it this way I think just to understand the value
// in doing it the way discussed in class. It is definately possible
// and not really to hard, but there is a lot to keep track of and a
// lot of ways to screw up. In a contest this would not be a good
// approach unless the other approach was unknown.
///////////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;

// Adjusts the value of the next node to connect if the current position
// is the 5th node in a direction.
int calc_val(int pos, int x) {
  if (pos % 5 == 4)
    return x - 5;
  return x;
}


int main() {
  int m,n,size,start,time, c=1;

  while (cin >> m >> n and m) {
    time = 1e9;  // Set initial time to an unreachable value
    size = n*m*20;  // Total nodes is cells * 20 nodes per cell
    vector<vector<int>> G(size);  // The graph
    vector<int> visited(size);  // Nodes that have been visited
    set<int> end;  // the nodes that we can finish on

    // Print endl between cases
    if (c != 1)
      cout << endl;

    // Build the graph
    for (int k = 0; k < n*m; k++) {
      // Get cell and deal with cell type
      char c;
      cin >> c;

      int cell = k*20;  // The north green node in the cell
      if (c == 'S') {
        start = cell;
      } else if (c == 'T') {  // Add all the good end nodes to end set
        end.insert(cell);
        end.insert(cell + 5);
        end.insert(cell + 10);
        end.insert(cell + 15);
      } else if (c == '#') {  // Mark blocked nodes as visited and do not conect them
        for (int i = 0; i < 20; i++) {
          visited[cell + i] = 1;
        }
        continue;
      }

      // Make connections for each of the 20 nodes in the cell
      for (int i = 0; i < 20; i++) {
        int pos = cell+i;  // The node number

        // Add internal connections representing changing direction
        if (i >= 0 and i < 5) {
          G[pos].push_back(pos + 5);
          G[pos + 5].push_back(pos);
          G[pos].push_back(pos + 15);
          G[pos + 15].push_back(pos);
        } else if (i >= 10 and i < 15) {
          G[pos].push_back(pos - 5);
          G[pos - 5].push_back(pos);
          G[pos].push_back(pos + 5);
          G[pos + 5].push_back(pos);
        }

        // Add External connections that represent changing cells
        // Each has a condition to make sure we don't try to add cells that
        // would put us off of the grid.
        // calc_val ensures proper wrap around for the 5 node in each color
        int val;
        if (i < 5) {  // Facing N
          val = calc_val(pos, (pos - (n * 20)) + 1);
          if (val >= 0) {
            G[pos].push_back(val);
          }
        } else if (i < 10) {  // Facing E
          val = calc_val(pos, (pos + 20) + 1);
          if (k % n != n - 1) {
            G[pos].push_back(val);
          }
        } else if (i < 15) {  // Facing S
          val = calc_val(pos, (pos + (n * 20)) + 1);
          if (val < size) {
            G[pos].push_back(val);
          }
        } else {  // Facing W
          val = calc_val(pos, (pos - 20) + 1);
          if (k % n != 0) {
            G[pos].push_back(val);
          }
        }
      }
    }

    // Do the easy bfs part
    queue<pair<int, int>> q;
    q.emplace(start, 0);
    visited[start] = 1;

    while (!q.empty()) {
      pair<int, int> p = q.front();
      q.pop();

      if (end.count(p.first)) {
        time = min(time, p.second);
        break;
      }

      for (auto i : G[p.first]) {
        if (!visited[i]) {
          q.emplace(i, p.second+1);
          visited[i] = 1;
        }
      }
    }

    // Print the result
    cout << "Case #" << c++ << endl;
    if (time == 1e9) {  // We did not reach the destination if time hasn't changed
      cout << "destination not reachable" << endl;
    } else {
      cout << "minimum time = " << time << " sec"<< endl;
    }
  }

  return 0;
}
