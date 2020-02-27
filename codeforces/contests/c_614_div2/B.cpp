#include <bits/stdc++.h>
using namespace std;

#define ll long long

int main() {
  double n;
  cin >> n;

  double ans = 0;
  for (int i = 1; i <= n; i++)
    ans += 1.0/i;

  cout << ans << endl;

  return 0;
}
