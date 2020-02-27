#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'

int get_msb(int n) {
  int ans = 0;
  while ((n = n >> 1)) ans++;
  return ans - 1;
}

int times_on(int bit, vector<int> A) {
  int ans = 0;
  for (auto n : A) ans += (n >> bit) & 1;
  return ans;
}

int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int n; cin >> n;
  vector<int> A(n);
  int mx = -1;
  int nz = 0;
  for (int i = 0; i < n; i++) {
    cin >> A[i];
    mx = max(mx, A[i]);
    if (A[i]) nz++;
  }

  int msb = get_msb(mx);
  int bit = -1;
  for (int b = msb; b >= 0; b--)
    if (times_on(b, A) == 1) {
      bit = b;
      break;
    }
  
  if (nz == 2) {
    sort(A.begin(), A.end(), greater<int>());
  } else if (bit >= 0) {
    for (int i = 0; i < n; i++) {
      if ((A[i] >> bit) & 1) {
        cout << A[i] << " ";
        A[i] = -1;
        break;
      }
    }
  }

  for (auto x : A) {
    if (x >= 0)
      cout << x << " ";
  }
  cout << endl;

  return 0;
}
