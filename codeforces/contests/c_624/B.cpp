#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'



int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int t; cin >> t;
  while (t--) {
    int n,m;
    cin >> n >> m;
    
    vector<int> A(n), P(m);
    for (auto& x : A) cin >> x;
    for (auto& x : P) cin >> x, x--;

    sort(P.begin(), P.end());

    int l = P[0], r = P[0];
    for (int i = 1; i < m; i++) {
      if (P[i] - P[i-1] > 1) {
        sort(A.begin() + l, A.begin() + r + 2);
        l = P[i];
      }
      r = P[i];
    }
    sort(A.begin() + l, A.begin() + r + 2);

    bool isSorted = true;
    for (int i = 1; i < n; i++) {
      if (A[i] < A[i-1]) {
        isSorted = false;
      }
    }

    if (isSorted)
      cout << "YES" << endl;
    else
      cout << "NO" << endl;

  }
  return 0;
}
