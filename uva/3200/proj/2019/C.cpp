#include <bits/stdc++.h>
using namespace std;

// We can build a small graph for the cube and just check the connections
// against the sides of a cube. Start at position 0 and if you find a connection
// that would not be possible return false. Otherwise it is true.

vector<vector<char>> G(8, vector<char>(8, '.'));


vector<vector<pair<int, int>>> moves{
  {{3, 2}, {2, 1}, {1, 0}},  // N
  {{4, 2}, {2, 5}, {5, 0}}   // E
  {{1, 2}, {2, 3}, {3, 0}},  // S
  {{5, 2}, {2, 4}, {4, 0}},  // W
}


int swap_bits(int a, int b, int n) {
  return (n & ~(1 << a) & ~(1 << b)) | (((n >> a) & 1) << b) | (((n >> b) & 1) << a);
}


int roll_cube(int cube, int dir) {
  int next = cube;
  for (auto& p : moves[dir]) {
    next = swap_bits(p.first, p.second, next);
  }

  if (next & 1)
    return -1;
  return next & 1;
}


vector<pair<int, int>> delta{
  {-1, 0}, {0, 1}, {0, -1}, {1, 0}
}


bool dfs(int r, int c, int cube) {
  bool ans = true;
  for (int i = 0; i < 4; i++) {
    int nr = r + delta[i].first;
    int nc = c + delta[i].second;
    int next = roll_cube(cube, i);

    if (next == -1)
      return false;
    else if (G[nr][nc] == '#' and !visited[nr][nc]) {
      visited[nr][nc];
      ans &= dfs(nr, nc, next);
    }
  }

  return ans;
}


int main() {
  int r = 0, c = 0;
  for (int i = 1; i <= 6; i++) {
    for (int j = 1; j <= 6; j++) {
      cin >> G[i][j];
      if (!r and G[i][j] == '#') {
        r = i;
        c = j;
      }
    }
  } 

  if (dfs(r, c, 1))
    cout << "can fold" << endl;
  else
    cout << "cannot fold" << endl;

  return 0;
}
