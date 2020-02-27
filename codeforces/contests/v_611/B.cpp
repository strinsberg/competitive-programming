#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'



int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int t;
  cin >> t;

  while (t--) {
    int n, k;
    cin >> n >> k;

    cout << min(k/2, n%k) + (n/k) * k << endl;

  }
  return 0;
}
