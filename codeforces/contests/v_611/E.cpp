#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'



int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int n;
  cin >> n;

  vector<int> B(n);
  for (int i = 0; i < n; i++)
    cin >> B[i];

  sort(B.begin(), B.end());

  vector<int> A;
  for (int i = 0; i < n; i++) {
    if (A.size() == 0 or B[i] != A.back())
      A.push_back(B[i]);
  }

  int mn = A.size();
  int mx = A.size();

  for (int i = 0; i < (int)A.size(); i++) {
    if (i < n - 1 and A[i] - A[i+1] == 1) {
      mn--;
      i++;
    }
  }

  // Would need to know group size to decide how many can be added
  // Ie) if group is 1 don't bother, 2 add only 1, 3+ add 2 if possible
  for (int i = 0; i < n; i++) {
    if (i == 0 and B[i] > 0)
      mx++;
    if (i == n - 1 and B[i] < n)
      mx++;
    else if (B[i] != B[i+1]) {
      if (B[i+1] - B[i] == 2)
        mx++;
      else if (B[i+1] - B[i] > 2)
        mx += 2;
    }
  }

  cout << mn << " " << mx << endl;
  return 0;
}
