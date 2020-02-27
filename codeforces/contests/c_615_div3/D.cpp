#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'

vector<int> counts;
set<pair<int, int>> mins;

// For each n we remove the pair of it with counts[y] and y
// Then we add a new pair for y with counts[y]++
// ans is counts_min * x + min
int solve(int n, int x) {
  int y = n % x;
  mins.erase(make_pair(counts[y]++, y));
  mins.insert(make_pair(counts[y], y));
  return mins.begin()->first * x + mins.begin()->second;
}


int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int q,x;
  cin >> q >> x;

  // Set up the data structures
  counts = vector<int>(x);
  for (int i = 0; i < x; i++) {
    mins.insert(make_pair(counts[i], i));
  }

  // Solve and print each query
  while (q--) {
    int n;
    cin >> n;
    cout << solve(n, x) << endl;;
  }
  return 0;
}
