#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'


int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int n;
  cin >> n;

  vector<int> D{100, 20, 10, 5, 1};
  ll ans = 0;

  for (auto i : D) {
    ans += n/i;
    n = n % i;
  }

  cout << ans << endl;

  return 0;
}
