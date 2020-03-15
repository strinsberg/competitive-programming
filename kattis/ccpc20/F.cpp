#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'



int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  string E,K,D; cin >> E >> K;

  for (int i = 0; i < (int)E.size(); i++) {
    int amt = K[i] - 'A';
    int ch = E[i] - 'A';
    if (i % 2 == 0) {
      D.push_back('A' + (26 + ch - amt) % 26);
    } else {
      D.push_back('A' + (ch + amt) % 26);
    }
  }

  cout << D << endl;

  return 0;
}
