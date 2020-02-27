#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'

int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int n, m;
  cin >> n >> m;

  // Fill matrix and reduce each element by 1
  vector<vector<int>> A(n, vector<int>(m));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      cin >> A[i][j];
      --A[i][j];
    }
  }

  ll ans = 0;

  // Check the number of moves needed for each shift
  for (int j = 0; j < m; j++) {
    vector<int> dist(n);

    for (int i = 0; i < n; i++) {
      if (A[i][j] % m == j) {  // Does it belong in this column
        int pos = A[i][j] / m;

        if (pos < n)  // There is a shift that could put this in it's place
          ++dist[(i - pos + n) % n];  // The number of moves this shift would require
      }
    }

    // Find the min number of moves required to get all numbers in position
    // By subtracting dist[i] from n we will get 0 if all numbers are in the right
    // order but we have to add i since this is how many we will move them to get
    // into place. If we chose a dist[i] with less numbers than n these will be added
    // as one move with the n - dis[i]. So it is the number of moves required to get
    // the numbers that require i moves + the number of numbers that will have to be replaced
    int mn = n - dist[0];
    for (int i = 1; i < n; i++) {
      mn = min(mn, n - dist[i] + i);
    }

    ans += mn;
  }

  cout << ans << endl;

  return 0;
}
