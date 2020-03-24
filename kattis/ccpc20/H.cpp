#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'
#define pii pair<int,int>


int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int n, mx = -1e9; cin >> n;
  vector<pair<int,int>> P(n);
  for (auto & x : P) {
    cin >> x.first >> x.second;
    mx = max(mx, x.first);
  }

  sort(P.begin(), P.end(), greater<pair<int,int>>());
  priority_queue<pii> q;

  ll ans = 0;
  for (int i = 0; i < (int)P.size(); i++) {
    if (P[i].first < mx) {
      while (mx > P[i].first) {
        if (!q.empty()) {
          ans += q.top().first;
          q.pop();
        }
        mx--;
      }
    }
    q.emplace(P[i].second, P[i].first);
  }

  while (mx) {
    if (!q.empty()) {
      ans += q.top().first;
      q.pop();
    }
    mx--;
  }

  cout << ans << endl;

  return 0;
}
