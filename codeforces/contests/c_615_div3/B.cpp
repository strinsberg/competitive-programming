#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'



int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int t;
  cin >> t;

  for (int k = 0; k < t; k++) {
    int n;
    cin >> n;

    vector<pair<int,int>> edges;
    for (int l = 0; l < n; l++) {
      int x, y;
      cin >> x >> y;
      edges.emplace_back(x,y);
    }

    sort(edges.begin(), edges.end());

    string path, ans("YES");
    int x = 0;
    int y = 0;
    for (auto& e : edges) {
      int dx = e.first - x;
      int dy = e.second - y;
      if (dx < 0 or dy < 0) {
        ans = "NO";
        break;
      }

      for (int i = 0; i < dx; i++) path.push_back('R');
      for (int i = 0; i < dy; i++) path.push_back('U');
      x = e.first;
      y = e.second;
    }
    
    cout << ans << endl;
    if (ans == "YES")
      cout << path << endl;
  }


  return 0;
}
