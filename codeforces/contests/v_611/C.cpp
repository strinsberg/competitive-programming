#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'



int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int n;
  cin >> n;
  
  int h = -1;
  vector<int> A(n);
  set<int> B;
  for (int i = 0; i < n; i++) {
    B.insert(i+1);
  }
  for (int i = 0; i < n; i++) {
    cin >> A[i];
    if (A[i] != 0) {
      B.erase(A[i]);
    } else {
      h = max(h, i+1);
    }
  }
  
  if (h == *B.begin()) {
    A[h-1] = *B.rbegin();
    B.erase(A[h-1]);
  }

  for (int i = 0; i < n; i++) {
    if (A[i] != 0) continue;

    if (i+1 == *B.rbegin()) {
      A[i] = *(++B.rbegin());
      B.erase(A[i]);
    } else {
      A[i] = *B.rbegin();
      B.erase(A[i]);
    }
  }

  for (int i = 0; i < n; i++) {
    cout << A[i];
    if (i < n - 1) cout << " ";
  }
  cout << endl;

  return 0;
}
