#include <bits/stdc++.h>
using namespace std;

int main () {
  vector<bool> event(366);
  int N;
  cin >> N;
  for (int i = 0; i < N; i++) {
    int s, t;
    cin >> s >> t;
    for (int j = s; j <= t; j++) {
      event[j] = true;
    }
  }

  int ans = 0;
  for (bool b : event) {
    ans += b;
  }

  cout << ans << endl;
  
  return 0;
}
