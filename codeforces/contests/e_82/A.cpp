#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'



int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int t; cin >> t;
  while (t--) {
    string s;
    cin >> s;
    ll ans = 0, remove = 0;
    bool one = false;
    for (auto c : s) {
      if (!one and c == '1') {
        one = true;
      } else if (one and c == '1') {
        ans += remove;
        remove = 0;
      }
      if (one and c == '0') {
        remove++;
      }
    }
    cout << ans << endl;
  }
  return 0;
}
