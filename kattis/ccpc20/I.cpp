#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'



int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int s,t,n; cin >> s >> t >> n;
  vector<int> D(n+1),B(n+1),C(n+1);
  for (int i = 0; i <= n; i++)
    cin >> D[i];

  for (int i = 1; i <=n; i++)
    cin >> B[i];

  for (int i = 1; i <=n; i++)
    cin >> C[i];

  int ans = s + D[0];
  for (int i = 1; i <= n; i++) {
    int x = C[i] - (ans % C[i]);
    ans += x == C[i] ? 0 : x;
    ans += D[i] + B[i];
  }

  cout << (ans > t ? "no" : "yes") << endl;

  return 0;
}
