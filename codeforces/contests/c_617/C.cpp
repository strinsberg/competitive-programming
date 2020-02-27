#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'

map<char, pair<int, int>> mv{
  {'L', {-1, 0}},
  {'R', {1, 0}},
  {'U', {0, 1}},
  {'D', {0, -1}},
};

int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int t;
  cin >> t;

  while (t--) {
    int n;
    cin >> n;

    map<pair<int,int>, int> A;
    pair<int,int> cur{0,0};
    A[cur] = 0;
    int l = -1, r = n;

    // Search for cycles and keep the range that is the smallest
    // we want to improve our route, but with the min adjustment
    for (int i = 1; i <= n; i++) {
      char ch;
      cin >> ch;

      auto m = mv[ch];
      cur.first += m.first;
      cur.second += m.second;

      if (A.count(cur) and i - (A[cur] + 1) < r - l) {
        l = A[cur] + 1;
        r = i;
      }
      A[cur] = i;
    }

    if (l == -1)
      cout << l << endl;
    else
      cout << l << " " << r << endl;
  }

  return 0;
}
