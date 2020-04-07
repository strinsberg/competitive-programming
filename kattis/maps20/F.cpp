#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'


string clean(string S) {
  for (auto& ch : S) {
    ch = tolower(ch);
    if (ch == '-')
      ch = ' ';
  }
  return S;
}


int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int n; cin >> n;
  cin.ignore();

  set<string> S;
  while (n--) {
    string line;
    getline(cin, line);

    S.insert(clean(line));
  }

  cout << S.size() << endl;

  return 0;
}
