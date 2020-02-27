#include <bits/stdc++.h>
using namespace std;

#define ll long long

int main() {
  int t;
  cin >> t;

  for (int i = 0; i < t; i++) {
    ll n,s,k;
    cin >> n >> s >> k;

    set<ll> closed;
    for (int j = 0; j < k; j++) {
      int c;
      cin >> c;
      closed.insert(c); 
    }

    ll inf = 1e12;
    ll low = 0;
    bool fl = false;
    ll high = 0;
    bool hl = false;
    for (int j = 0; j < 1020; j++) {
      if (!fl && s - j < 1) {
        low = inf;
        fl = true;
      }

      if (closed.count(s - j) and !fl) {
        low++;
      } else {
        fl = true;
      }

      if (!hl && s + j > n) {
        high = inf;
        hl = true;
      }

      if (closed.count(s + j) and !hl) {
        high++;
      } else {
        hl = true;
      }

      if (hl and fl)
        break;
    }

    cout << min(low, high) << endl;
  }

  return 0;
}
