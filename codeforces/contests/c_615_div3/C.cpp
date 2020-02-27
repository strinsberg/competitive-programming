#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'



int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int t;
  cin >> t;

  while (t-- != 0) {
    ll n;
    cin >> n;

    vector<ll> divisors;
    for (ll i = 2; i*i <= n; i++) {
      if (n % i == 0) divisors.push_back(i);
    }

    string ans = "NO";
    ll a,b,c;
    for (ll i = 0; i < (int)divisors.size(); i++) {
      for (ll j = i + 1; j < (int)divisors.size(); j++) {
        a = divisors[i];
        b = divisors[j];
        if (a == b) break;
        ll t = a * b;
        if (t < n) {
          if (n % t == 0 and n/t != a and n/t != b) {
            ans = "YES";
            c = n / t;
            goto done;
          }
        }
      }
    }

    done:
      cout << ans << endl;
      if (ans == "YES")
        cout << a << " " << b << " " << c << endl;
  }

  return 0;
}
