#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'

vector<int> A;

// This is 2^n so TLE
// Need to do it with a dp table and without recursion probably
// or need a totally different algorithm
int solve(int b, int i, int k) {
  if (b < 0) {
    return k - 1;
  } else if (b == 0 or i < 0) {
    return k;
  } else {
    int l, r;
    l = solve(b - A[i], i - 1, k + 1);
    r = solve(b, i - 1, k);
    return max(l, r);
  }
}

int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);
  int t; cin >> t;
  int c = 1;
  
  while (t--) {
    int n,b; cin >> n >> b;
    A = vector<int>(n);
    for (auto& x : A) cin >> x;
    
    cout << "Case #" << c++ << ": " << solve(b, n - 1, 0) << endl;
  }

  return 0;
}
