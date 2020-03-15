#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'



int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int n; cin >> n;
  vector<pair<int,int>> P(n);
  for (auto & x : P) cin >> x.first >> x.second;

  map<int,int> D;
  for (auto & x : P) {
    D[x.first] = max(D[x.first], x.second);
  }

  ll ans = 0;
  for (auto & x : D) {
    ans += x.second;
  }

  cout << ans << endl;

  return 0;
}
