#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'



int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  ll n, a, b, k;
  cin >> n >> a >> b >> k;

  vector<ll> M(n);
  for(int i = 0; i < n; i++) {
    cin >> M[i];
    M[i] = M[i] % (a + b);
    if (M[i] == 0) M[i] = a+b;
  }

  sort(M.begin(), M.end());

  ll me = 0;
  for (auto i : M) {
    if (i > 0 and i <= a)
      me++;
    else {
      ll r = (i == 0 ? (a + b) : i) - a;
      ll d = r / a;
      if (r % a != 0) d++;
      if (k - d >= 0) {
        k-=d;
        me++;
      }
    }
  }

  cout << me << endl;

  return 0;
}
