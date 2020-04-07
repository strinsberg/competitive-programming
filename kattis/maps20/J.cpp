#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'


long double solve(vector<int>& S, int i) {
  long double ans = 0;
  long double drop = 0;
  for (int j = 0; j < (int)S.size(); j++) {
    if (j == i) {
      drop = 1;
      continue;
    }
    ans += S[j] * pow(0.8, j - drop);
  }
  return 0.2 * ans;
}


int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int n; cin >> n;
  vector<int> S(n);
  for (auto& x : S) cin >> x;

  long double total = 0;
  for (int i = 0; i < n; i++) {
    total += solve(S, i);
  }

  cout << setprecision(9) << fixed << solve(S, -1) << endl;
  cout << total / S.size() << endl;


  return 0;
}
