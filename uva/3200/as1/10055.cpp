///////////////////////////////////////////////////////////////////////
// CPSC 3200
// Assignment 1
// UVA 10055
//
// Steven Deutekom
// Jan 6 2020
//
// Caclculates the difference with abs. This ensures it will always
// be positive since we don't know which input is Hashmat's army.
///////////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;

int main() {
  long long x, y;
  while (cin >> x >> y && cin) {
    cout << abs(x - y) << endl;
  }

  return 0;
}

