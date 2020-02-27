#include <bits/stdc++.h>
using namespace std;

int main() {
  int t;
  cin >> t;

  for (int i = 0; i < t; i++) {
    int n;
    cin >> n;

    int yasser = 0;
    vector<int> cakes(n);
    for (int j = 0; j < n; j++) {
      cin >> cakes[j];
      yasser += cakes[j];
    }
    
    vector<int> taste(n+1, 0);
    for (int j = 0; j < n; j++) {
      taste[j+1] = taste[j] + cakes[j];
    }

    string ans = "YES";
    for (int j = 0; j < n; j++) {
      for (int k = j+1; k <= n; k++) {
        int s = taste[k] - taste[j];
        if (s >= yasser && k - j != n) {
          ans = "NO";
        }
      }
    }

    cout << ans << endl;

  }
  return 0;
}
