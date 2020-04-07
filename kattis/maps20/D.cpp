#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'

#define PI 3.1415926535897932


long double solve(long double n, long double l, long double d, long double g) {
  long double a = (l / 2.0) / tan((360.0 / (2 * n)) * PI / 180.0);
  long double area = n * ((l / 2.0) * a);
  long double p = d * g;
  long double extra = (n * (p * l)) + (PI * p * p);
  return area + extra;
}


int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int t; cin >> t;
  while (t--) {
    long double n,l,d,g; cin >> n >> l >> d >> g;

    cout << setprecision(9) << fixed << solve(n,l,d,g) << endl;
  }

  return 0;
}
