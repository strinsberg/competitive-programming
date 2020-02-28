#include <bits/stdc++.h>
using namespace std;

int calc_val(int pos, int x) {
  if (pos % 5 == 4)
    return x - 5;
  return x;
}


int main() {
  int m,n,size,start,time, c=1;

  while (cin >> m >> n and m) {
    time = 1e9;
    size = n*m*20;
    vector<vector<int>> G(size);
    vector<int> visited(size);
    set<int> end;

    if (c != 1)
      cout << endl;

    // Build the graph
    for (int k = 0; k < n*m; k++) {
      // Get cell and deal with cell type
      char c;
      cin >> c;

      int cell = k*20;
      if (c == 'S') {
        start = cell;
      } else if (c == 'T') {
        end.insert(cell);
        end.insert(cell + 5);
        end.insert(cell + 10);
        end.insert(cell + 15);
      } else if (c == '#') {
        for (int i = 0; i < 20; i++) {
          visited[cell + i] = 1;
        }
        continue;
      }

      // Make connections
      for (int i = 0; i < 20; i++) {
        int pos = cell+i;  // The node number

        // Add internal connections
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

        // Add External connections
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

    // Do the bfs
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
    if (time == 1e9) {
      cout << "destination not reachable" << endl;
    } else {
      cout << "minimum time = " << time << " sec"<< endl;
    }
  }

  return 0;
}
