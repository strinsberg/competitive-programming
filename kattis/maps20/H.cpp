#include <bits/stdc++.h>
using namespace std;

#define endl '\n'

int to_cap(int n, int c) {
  int i = 0;
  for (; n <= c ; i++, n*=2) {}
  return i - 1;
}


long long solve(int q, int c, vector<int>& F) {
  long long ans = 0;
  for (auto x : F) {
    if (x > c) {
      ans += 1ll << max(0, q - to_cap(x/2, c));
      ans += 1ll << max(0, q - to_cap((x + c) / 2, c));
    } else {
      ans += 1ll << max(0, q - to_cap(x, c));
    }
  }
  return ans;
}


int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int c,n,m; cin >> c >> n >> m;
  
  vector<int> F(n);
  for (auto& x : F) cin >> x;

  vector<int> Q(m);
  for (auto& x : Q) cin >> x;

  for (auto q : Q)
    cout << solve(q, c, F) << endl;


  return 0;
}
