#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'



int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int t; cin >> t;
  while (t--) {
    int n, zeros = 0; cin >> n;
    ll sum = 0;
    for (int i = 0; i < n; i++) {
      int x;
      cin >> x;
      sum += x;
      if (x == 0) zeros++;
    }

    ll ans = 0;
    ans += zeros;
    sum += zeros;
    if (sum == 0)
      ans += 1;

    cout << ans << endl;   

  }

  return 0;
}
