#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'



int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int t;
  cin >> t;

  while (t--) {
    int x;
    string n;

    cin >> x;
    cin >> n;

    int odd = 0, even = 0;
    for (auto ch : n) {
      cout << ch - '0' << endl;
      if (ch - '0' % 2 == 0)
        even++;
      else
        odd++;
    }

    cout << odd << " " << even << endl;

    if (!even) {
      cout << -1 << endl;
      continue;
    }

    if (n.back() - '0'  % 2 != 0 and odd % 2 == 0) {
      cout << n << endl;
    }

    while (n.back() - '0' % 2 == 0) {
      n.pop_back();
    }

    if (odd % 2 != 0) {
      for (int i = 0; i < (int)n.size(); i++) {
        if ((n[i] - '0') % 2 != 0)
          n.erase(i, 1);
      }
    }

    cout << n << endl;
  }

  return 0;
}
