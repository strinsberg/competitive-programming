#include <bits/stdc++.h>
using namespace std;
 
#define ll long long
#define endl '\n'
 
 
int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);
 
  int t; cin >> t;
  while (t--) {
    int n,m; cin >> n >> m;
    string S; cin >> S;
    vector<int> P(m);
    for (auto& x : P) cin >> x, x--;
 
    sort(P.begin(), P.end());
    map<char, ll> ans;
 
    m++;
    int tries = 0;
    for (int i = 0; i < n and m > 0; i++) {
      ans[S[i]] += m;
      while (P[tries] == i) {  // This is an error. What happens if tries is out of bounds? How does this happen?
        m--;
        tries++;
      }
    }
 
    for (char c = 'a'; c <= 'z'; c++)
      cout << ans[c] << " ";
    cout << endl;  
 
  }
  return 0;
}
