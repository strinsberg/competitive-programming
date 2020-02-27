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

    ll ans = 0;
    do {
      ans += n - n % 10;
      n = (n / 10) + (n % 10);
    } while (n >= 10);


    cout << ans + (n % 10) << endl;
  }

  return 0;
}
