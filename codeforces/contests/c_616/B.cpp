#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'

int inc(vector<int>& A) {
  for (int i = 0; i < (int)A.size(); i++) {
    if (A[i] < i)
      return i - 1;
  }
  return A.size();
}

int dec(vector<int>& A) {
  for (int i = (int)A.size() - 1; i >= 0; i--) {
    if (A[i] < A.size() - i - 1)
      return i + 1;
  }
  return 0;
}


int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int t;
  cin >> t;

  while (t--) {
    int n;
    cin >> n;

    vector<int> A(n);
    for (int i = 0; i < n; i++)
      cin >> A[i];

    int h = inc(A);
    int l = dec(A);
    if (h >= l)
      cout << "Yes" << endl;
    else
      cout << "No" << endl;

  }
  return 0;
}
