#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'



int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int t;
  cin >> t;

  while (t--) {
    int h, m;
    cin >> h >> m;

    cout << ((24 - h) * 60) -  m << endl;

  }
  return 0;
}
