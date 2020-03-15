#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'

#define MAX 1005

int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int n; cin >> n;

  for (int i = 0; i < n; i++) {
    int m; cin >> m;
    
    vector<int> V(MAX, 0);
    vector<int> fib(MAX, 1);
    for (int i = 2; i < MAX; i++) {
      fib[i] = (fib[i-1] + fib[i-2]) % m;
      if (V[fib[i]] > 0) {
        cout << V[fib[i]] << endl;;
        break;
      } else {
        V[fib[i]] = i;
      }
    }
  }

  return 0;
}
