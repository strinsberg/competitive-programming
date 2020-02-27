#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'

int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int t;
  cin >> t;

  while(t--) {
    int n, x, xc = 0;
    cin >> n >> x;
    
    string S;
    cin >> S;
    vector<int> X(n);
    for (int i = 0; i < n; i++) {
      X[i] = S[i] == '1' ? -1 : 1;
      if (i) X[i] = X[i-1] + X[i];
      if (X[i] == x) xc++;
    }

    ll ans = 0;
    ll d = X.back();
    if (!d) {
      if (!x or xc)
        ans--;
    } else if (d > 0) { 
      if (x == 0) ans++;
      for (auto i : X) {
        if (i <= x and (i-x) % d == 0) ans++;
      }
    } else {
      if (x == 0) ans++;
      for (auto i : X) {
        if (i >= x and (i-x) % d == 0) ans++;
      }
    }

    cout << ans << endl;
  }

  return 0;
}
