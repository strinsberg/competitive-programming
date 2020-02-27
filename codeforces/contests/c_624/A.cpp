#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'


int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int t; cin >> t;
  while(t--) {
   int a,b;
   cin >> a >> b;
   int k = b - a;
   if (k == 0) {
      cout << 0 << endl;
   } else if (k > 0) {
      if (k % 2 == 0)
        cout << 2 << endl;
      else
        cout << 1 << endl;
   } else {
      if (k % 2 == 0)
        cout << 1 << endl;
      else
        cout << 2 << endl;
   }
  }
  return 0;
}
