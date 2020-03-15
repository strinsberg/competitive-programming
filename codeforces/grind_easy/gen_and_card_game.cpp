#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'



int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  char r,s; cin >> r >> s;

  vector<char> R(5),S(5);
  for (int i = 0; i < 5; i++) {
    cin >> R[i] >> S[i];
    if (R[i] == r or S[i] == s) {
      cout << "YES" << endl;
      return 0;
    }
  }

  cout << "NO" << endl;

  return 0;
}
