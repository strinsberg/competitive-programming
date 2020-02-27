#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'



int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int n; cin >> n;
  char a = 0, b = 0;
  string ans;

  // if the next char is >= a add a 0, else add 1if b >= a, else not possible
  // My intuition is that if there is a subsequence of 3 chars that are in
  // decreaseing order it is not possible. Ie) edc
  for (int i = 0; i < n; i++) {
    char ch; cin >> ch;

    if (ch >= a) a = ch, ans+='0';
    else if (ch >= b) b = ch, ans+='1';
    else return cout << "NO" << endl, 0;
  }

  cout << "YES" << endl << ans << endl;

  return 0;
}
