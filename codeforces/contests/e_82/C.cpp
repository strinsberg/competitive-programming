#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'



int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int t; cin >> t;
  while (t--) {
    string s; cin >> s;
    map<char, set<char>> A;
    for (int i = 0; i < s.size(); i++) {
      if (i == 0 or i == s.size()-1) {
        A[s[i]].insert('~');
        continue;
      }
      A[s[i]].insert(s[i-1]);
      A[s[i]].insert(s[i+1]);
      A[s[i-1]].insert(s[i]);
      A[s[i+1]].insert(s[i]);
    }

    string ans = "YES";
    char start;
    for (auto& a : A) {
      if (a.second.size() > 2) {
        ans = "NO";
        break;
      } if (a.second.count('~')) {
        start = *(++a.second.begin());
      }
    }

    set<char> printed;
    printed.insert('~');
    cout << ans << endl;
    if (ans == "YES") {
      cout << start;
      printed.insert(start);
      char next = *(A[start].begin());
      char last = start;
      do {
        cout << next;
        printed.insert(next);
        char c = *(A[next].begin());
        c = c != last ? c : *(++A[next].begin());
        last = next;
        next = c;
      } while (A.count(next) and !printed.count(next));
      /*
      for (auto c : "abcdefghijklmnopqrstuvwxyz") {
        if (!A.count(c)) cout << c;
      }
*/
      cout << endl;
    }

  }
  return 0;
}
