#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'



int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int n, x;
  cin >> n;
  x = n;

  stringstream ss;

  while (--x) {
    if (n%2)
      ss << (x%2 == 0 ? "I hate that " : "I love that ");
    else
      ss << (x%2 == 1 ? "I hate that " : "I love that ");
  }

  ss << (n%2 == 1 ? "I hate it" : "I love it");

  cout << ss.str() << endl;

  return 0;
}
