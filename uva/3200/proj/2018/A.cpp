///////////////////////////////////////////////////////////////////////
// CPSC 3200
// Project
// rmc18 Problem A -> Quality-Adjusted Life-Year
//
// Steven Deutekom
// March 2 2020
//
// Simply apply the formula given in the question.
// QALY is sum(quality * years).
///////////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;

int main() {
  int n;
  cin >> n;

  double a, b, ans = 0.0;
  for (int i = 0; i < n; i++) {
    cin >> a >> b;
    ans += a * b; 
  }

  cout << ans << endl;

  return 0;
}
