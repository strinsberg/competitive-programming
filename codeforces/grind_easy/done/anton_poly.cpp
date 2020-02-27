#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'



int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  map<string, int> sides{
    {"Tetrahedron", 4},
    {"Cube", 6},
    {"Octahedron", 8},
    {"Dodecahedron", 12},
    {"Icosahedron", 20},
  };

  ll ans = 0;
  int n;
  cin >> n;

  while (n--) {
    string poly;
    cin >> poly;
    ans+=sides[poly];
  }

  cout << ans << endl;

  return 0;
}
