#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'

int n,l,h;

bool solve(int n, bool alex) {
  int left = n - max(l, min(h, n-h-l));
  if (left < l)
    return alex;
  else if ((left >= l and left <= h) or left - h < l)
    return !alex;
  else
    return solve(left, !alex);
}

int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  cin >> n >> l >> h;
  // need to remove even number of h's from n till there are 2*h + n%h left
  cout << (solve(, true) ? "Alex" : "Barb") << endl;

  return 0;
}
