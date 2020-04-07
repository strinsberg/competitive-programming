#include <bits/stdc++.h>
using namespace std;

int main() {
  int t; cin >> t;
  int c = 1;

  while (t--) {
    int n; cin >> n;

    vector<pair<int,pair<int,int>>> S;
    for (int i = 0; i < n; i++) {
      int x,y; cin >> x >> y;
      S.push_back({x, {y, i}});
    }
    sort(S.begin(), S.end());

    string ans;
    vector<pair<int, char>> A;
    pair<int,int> last_c = {-1,-1}, last_j = {-1,-1};

    for (auto& x : S) {
      if (last_c.second <= x.first) {
        A.emplace_back(x.second.second, 'C');
        last_c = {x.first, x.second.first};
      } else if (last_j.second <= x.first) {
        A.emplace_back(x.second.second, 'J');
        last_j = {x.first, x.second.first};
      } else {
        ans = "IMPOSSIBLE";
        break;
      }
    }

    if (ans != "IMPOSSIBLE") {
      sort(A.begin(), A.end());
      for (auto& x : A) {
        ans+=x.second;
      }
    }
    cout << "Case #" << c++ << ": " << ans << endl;
  }
  return 0;
}
