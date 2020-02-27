#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'


pair<int, int> oe(string& s) {
  int o = 0, e = 0;
  for (auto ch : s) {
    if ((ch -'0') % 2 == 0)
      e++;
    else
      o++;
  }
  return {o,e};
}


int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int t;
  cin >> t;

  while (t--) {
    int x;
    string n;

    cin >> x;
    for (int i = 0; i < x; i++) {
      char ch;
      cin >> ch;
      if ((ch - '0') % 2 != 0)
        n.push_back(ch);
    }

    if (n.size() % 2 != 0)
      n.pop_back();

    if (n.size() > 0)
      cout << n << endl;
    else
      cout << -1 << endl;
  }

  return 0;
}
