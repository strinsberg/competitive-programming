#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
template<typename T>
T bpow(T x, ll n) { return n ? n % 2 ? x * bpow(x, n - 1) : bpow(x * x, n / 2) : T(1); }
template<typename T>
T gcd(const T &a, const T &b) { return b == T(0) ? a : gcd(b, a % b); }


// My Code ////////////////////////////////////////////////////////////

bool possible(ll a, ll b, ll X) {
  ll cm = a*b / gcd(a,b);
  if (a*b != cm or cm != X)
    return false;

  return true;
}

int main() {
  ll X;
  cin >> X;

  // Find all prime factors of x and their exponents
  ll m = X;
  vector<pair<ll, ll>> factors;
  for (ll i = 2, a = 0; m > 1 and i*i <= X; i++, a = 0) {
    while (m > 1 and m % i == 0) m /= i, a++;
    if (a > 0) factors.push_back({i, a});
  }
  if (m > 1) factors.push_back({m, 1});

  // Loop through all subsets
  pair<ll, ll> ans;
  ll mn = 1e18;
  for (ll i = 0; i < (1<<factors.size()); i++) {
    ll a = 1, b = 1;
    for (int j = 0; j < factors.size(); j++) {
      if (i & (1<<j))
        a *= bpow(factors[j].first, factors[j].second);
      else
        b *= bpow(factors[j].first, factors[j].second);
    }

    // If a and b are possible check to see if they are the answer
    if (possible(a, b, X)) {
      ll mx = max(a,b);
      if (mx < mn) {
        mn = mx;
        ans = {a, b};
      }
    }
  }

  cout << ans.first << " " << ans.second << endl;

  return 0;
}
