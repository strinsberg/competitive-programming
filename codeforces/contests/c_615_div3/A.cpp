#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'



int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int t;
  cin >> t;

  for (int j = 0; j < t; j++) {
    ll A,B,C,n;
    cin >> A >> B >> C >> n;

    ll mx, md, mn;
    if (A > B) {
      mx = A;
      mn = B;
    } else {
      mx = B;
      mn = A;
    }

    if (C > mx) {
      md = max(mx, mn);
      mn = min(mx, mn);
      mx = C;
    } else {
      md = max(C, mn);
      mn = min(C, mn);
    }

    ll diff = 0;
    diff += mx - mn;
    diff += mx - md;

    string ans = "YES";
    if (diff > n or (n - diff) % 3 != 0)
      ans = "NO";

    cout << ans << endl; 
  }

  return 0;
}
