#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'

vetor<int> dp;
int fib(int m) {
  dp[0] = dp[1] = 1;
  for (int 
}

void solve(int n) {
  int ans = 0;
  


  cout << ans << endl;
}


int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int n; cin >> n;

  for (int i = 0; i < n; i++) {
    int x; cin >> x;
    solve(x);
  }

  return 0;
}
