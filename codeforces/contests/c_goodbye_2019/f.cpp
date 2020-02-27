#include <bits/stdc++.h>
using namespace std;

int main() {
  string s;
  cin >> s;  

  vector<int> sums(s.size() + 1, 0);
  for (int i = 0; i < s.size(); i++) {
    sums[i+1] = sums[i];
    if (s[i] == '1')
      sums[i+1]++;
  }

  int ans = sums[s.size()];
  for (int l = 2; l <= s.size(); l++) {
    for (int i = 0; i <= s.size() - l; i++) {
      int sum = sums[i + l] - sums[i];
      if (sum > 0 && l % sum == 0) {
        ans++;
      }
    }
  }

  cout << ans << endl;

  return 0;
}
