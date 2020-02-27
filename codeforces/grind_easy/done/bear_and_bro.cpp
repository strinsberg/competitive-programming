#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'



int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  ll a, b, ans = 0;
  cin >> a >> b;
  while (a <= b) {
    a *= 3;
    b *= 2;
    ans++;
  }

  cout << ans << endl;

  return 0;
}
