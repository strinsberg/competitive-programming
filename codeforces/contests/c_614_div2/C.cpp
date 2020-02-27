#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'

// Find out how many blocked pairs a given cell would make
int blocked(int r, int c, vector<vector<bool>>& map) {
  r = r ? 0 : 1;
  int ans = 0;
  for (int i = -1; i<=1; i++) {
    if (c+i < 0 or c+i >= map[0].size()) continue;
    if (map[r][c+i])
      ans++;
  }
  return ans;
}


int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);
  int n, q;
  cin >> n >> q;

  vector<vector<bool>> map(2, vector<bool>(n, false));

  int blocks = 0;
  for (int i = 0; i < q; i++) {
    // Get cell to switch
    int r, c;
    cin >> r >> c;
    r--, c--;

    // If a cell is blocked subtract the number of blocked pairs from total
    // else add the number of blocked pairs
    if (map[r][c]) {
      map[r][c] = false;
      blocks -= blocked(r,c,map);
    } else {
      map[r][c] = true;
      blocks += blocked(r,c,map);
    }
  
    // Print Yes if there are no blocks
    if (blocks == 0)
      cout << "Yes" << endl;
    else
      cout << "No" << endl;
  }

  return 0;
}
