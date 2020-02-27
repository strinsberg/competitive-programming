///////////////////////////////////////////////////////////////////////
// CPSC 3200
// Assignment 3
// UVA 11136
//
// Steven Deutekom
// Jan 10 2020
//
// Insert all bills into a multiset. This allows quick access to the
// min and max bills. Take those two bills and subtract them and add
// the result to the total paid out. Then remove the bills from the
// multiset and continue.
///////////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;

int main() {
  int n;

  while (cin >> n && n) {
    long long total = 0;
    multiset<int> bills;

    // Deal with a days bills and payouts
    for (int i = 0; i < n; i++) {
      int k;
      cin >> k;

      // Get all bills for the day
      for (int j = 0; j < k; j++) {
        int b; cin >> b;
        bills.insert(b);
      }

      // Do payout computation and remove bills
      auto l = bills.begin();
      auto h = --bills.end();
      total += *h - *l;
      bills.erase(l);
      bills.erase(h);
    }
    cout << total << endl;
  }
  return 0;
}
