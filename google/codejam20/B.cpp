#include <bits/stdc++.h>
using namespace std;

int main() {
  int t; cin >> t;
  int c = 1;

  while (t--) {
    string s; cin >> s;
    stringstream ss;

    char last = 0;
    int placed = 0;
    for (auto& c : s) {
      int x = c - '0';

      if (x == last) {
        ss << x;
      } else if (x > last) {
        for (int i = 0; i < abs(x - last); i++) {
          ss << '(';
          placed++;
        }
        ss << x; 
      } else {
        for (int i = 0; i < abs(last - x); i++) {
          ss << ')';
          placed--;
        }
        ss << x;
      }
      last = x;
    }

    for (int i = 0; i < placed; i++)
      ss << ')';

    cout << "Case #" << c++ << ": " << ss.str() << endl;
  }
  return 0;
}
