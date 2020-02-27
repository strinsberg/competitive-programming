#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'



int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int t;
  cin >> t;

  while (t--) {
    int n;
    cin >> n;

    int o = 0, e = 0;
    for (int i = 0; i < n; i++) {
      int x;
      cin >> x;
      if (x % 2 == 0)
        e++;
      else
        o++;
    }

    string ans = "NO";
    if (o % 2 == 0) {
      if (e > 0 and o > 0)
        ans = "YES";
    } else
      ans = "YES";

    cout << ans << endl;

  }

  return 0;
}
