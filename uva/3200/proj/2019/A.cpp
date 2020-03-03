///////////////////////////////////////////////////////////////////////
// CPSC 3200
// Project
// rmc19 probnem A -> Piece of Cake!
//
// Steven Deutekom
// March 2 2020
//
// Calculate the area of each piece of cake. Sort them and output the
// largest one * thickness.
///////////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;


int main () {
  int n,h,v;
  cin >> n >> h >> v;

  vector<int> ans{h*v, h*(n-v), (n-h)*v, (n-h)*(n-v)};
  sort(ans.begin(), ans.end());

  cout << ans.back() * 4 << endl;
  return 0;
}
