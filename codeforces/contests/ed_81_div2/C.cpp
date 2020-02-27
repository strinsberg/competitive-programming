#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'


int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int t;
  cin >> t;

  while (t--) {
    string S, T;
    cin >> S >> T;

    // Build a sequence table setting where each char in S points to
    // the next occurence of each char in the alphabet
    int n = S.size(), m = T.size();
    vector<vector<int>> seq(256, vector<int>(n, -1));
    for (int ch = 'a'; ch <='z'; ch++) {
      for (int i = n - 1; i >= 0; i--) {
        if (S[i] == ch)
          seq[ch][(n + i - 1) % n] = i;
        else
          seq[ch][(n + i - 1) % n] = seq[ch][i];
      }

      for (int i = n - 1; i >= 0; i--) {
        if (seq[ch][i] == -1)
          seq[ch][i] = seq[ch][(i+1) % n];
      }
    }
      
    // Find each subsequence needed to make the string T
    ll ans = 0;
    for (int i = 0, j = n-1; i < m; i++) {
      if (seq[T[i]][j] == -1) {
        ans = -1;
        break;
      }

      if (seq[T[i]][j] <= j) ans++;
      j = seq[T[i]][j];
    }

    cout << ans << endl;
  }

  return 0;
}
