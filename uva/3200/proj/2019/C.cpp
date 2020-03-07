///////////////////////////////////////////////////////////////////////
// CPSC 3200
// Project
// rmc19 Problem C -> Folding a Cube
//
// Steven Deutekom
// March 4 2020
//
// My approach is to place a cube (saved in a bitmask) on the first # i see.
// I then DFS the pattern made by the #s keeping track of all sides that
// have touched the ground. In the dfs we pick a neighbor and roll the cube
// in that direction getting a new cube pattern. If at any point the cube
// is rolled onto an already used side the attempt fails. When we get the
// result back from a DFS on a neighbor we roll it back and add all new
// sides that have been used to the cube. Then pass roll and pass this cube
// to any more DFSs. If at the end of the dfs the result is not -1 then we
// have succeeded.
//
// There is probably an easier way to do this instead of using bits, but
// I wanted to give it a try, because bits are fun ;)
///////////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;

// Graph and visited array. Padded with '.' to avoid bounds checking.
vector<vector<char>> G(8, vector<char>(8, '.'));
vector<vector<bool>> V(8, vector<bool>(8, false));


// The bits to switch around when rollong the cube in each direction
// They must be done in order to circular shift the bits properly.
vector<vector<pair<int, int>>> moves{
  {{3, 2}, {2, 1}, {1, 0}},  // N
  {{5, 2}, {2, 4}, {4, 0}},  // E
  {{1, 2}, {2, 3}, {3, 0}},  // S
  {{4, 2}, {2, 5}, {5, 0}}   // W
};


// Swap 2 bits in a bitmask. Might be able to make it a bit shorter, but
// this approach does work.
int swap_bits(int a, int b, int n) {
  return (n & ~(1 << a) & ~(1 << b)) | (((n >> a) & 1) << b) | (((n >> b) & 1) << a);
}


// Returns a pattern of the cube after rolling. 0 is always the side down and
// is turned on after the swapping of all sides. If 0 is already on after
// the swaps it means it was already used and we fail.
// Unroll is for coming back out of the DFS when we know we will put already
// used sides down. If unroll we ignore the check and just return the rolled cube.
int roll_cube(int cube, int dir, bool unroll) {
  int next = cube;
  for (auto& p : moves[dir]) {
    next = swap_bits(p.first, p.second, next);
  }

  if (!unroll and next & 1) {
    return -1;
  } return next | 1;
}


vector<pair<int, int>> delta{
  {-1, 0}, {0, 1}, {1, 0}, {0, -1}
};


// Dfs the cube pattern and return the cube with its sides marked if they
// have been used.
int dfs(int r, int c, int cube) {
  int next = cube;  // To keep track of the sides that have been used in subtrees

  // Try each direction
  for (int i = 0; i < 4; i++) {
    int nr = r + delta[i].first;
    int nc = c + delta[i].second;

    // If there is a cube side here and we have not visited it yet try it
    if (G[nr][nc] == '#' and !V[nr][nc]) {
      // dont dfs if the roll will fail
      int roll = roll_cube(next, i, false);
      if (roll == -1)
        return -1;

      V[nr][nc] = true;
      // Unroll the result and add the sides that have been used
      next |= roll_cube(dfs(nr, nc, roll), (i + 2) % 4 , true);
    }
  }

  return next;
}


int main() {
  int r = -1, c = -1;

  // read in the pattern
  for (int i = 1; i <= 6; i++) {
    for (int j = 1; j <= 6; j++) {
      cin >> G[i][j];

      // Mark the first # we came to as the start
      if (r == -1 and G[i][j] == '#') {
        r = i;
        c = j;
      }
    }
  } 

  V[r][c] = true;
  if (dfs(r, c, 1) != -1)
    cout << "can fold" << endl;
  else
    cout << "cannot fold" << endl;

  return 0;
}
