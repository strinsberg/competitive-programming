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

    string ans = "";
    if (n % 2) ans.push_back('7'), n-=3;

    cout << ans << string(n/2, '1') << endl;
  }

  return 0;
}
