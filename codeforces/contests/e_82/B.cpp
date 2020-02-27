#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'



int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int t; cin >> t;
  while (t--) {
    ll n,g,b;
    cin >> n >> g >> b;

    ll goal = (n / 2);
    if (n%2 == 1) goal++;
    ll ans = 0;

    if (goal <= g) {
      ans = n;
    } else {
      ans = (goal/g) + (goal%g);
      if (goal % g == 0)
        ans += ((goal/g) - 1) * b;
      else
        ans += (goal/g)*b;
    }
    if (ans < n) ans = n;
    cout << ans << endl;

  }
  return 0;
}
