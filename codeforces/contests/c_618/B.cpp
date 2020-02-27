#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'



int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int t; cin >> t;
  while(t--) {
    int n; cin >> n;
    vector<int> A(2*n);
    for (int i = 0; i < 2*n; i++) {
      cin >> A[i];
    }

    sort(A.begin(), A.end());
    cout << abs(A[n-1] - A[n]) << endl;
  }

  return 0;
}
