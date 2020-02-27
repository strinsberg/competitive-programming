///////////////////////////////////////////////////////////////////////
// CPSC 3200
// Project -> Rocky Mountain Regional 2011
// Stock Prices
//
// Steven Deutekom
// Jan 13 2020
//
// ** Does Not Pass on ICPC Live Archive, But You ran it on your data
// and said it is passing. **
//
// Put the prices and the days into pairs (value, day). Sort this list
// to find the k1 min and k2 max prices. Collect those days and sort
// them before printing to be in the desired order.
///////////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;

// Return a vector with k days from a vector of stock prices
vector<int> getk(int k, vector<pair<int,int>>& prices) {
  vector<int> kp;
  for (int i = 0; i < k; i++)
    kp.push_back(prices[i].second);
  sort(kp.begin(), kp.end());
  return kp;
}

// Solves the problem ;)
int main() {
  int n, l, h, c = 1;
  
  while (cin >> n >> l >> h && n) {
    // Get all the prices
    vector<pair<int, int>> prices;
    for (int i = 1; i <= n; i++) {
      int p;
      cin >> p;
      prices.emplace_back(p, i);
    }

    cout << "Case " << c++ << endl;

    // Print our l lowest prices
    sort(prices.begin(), prices.end());
    vector<int> low = getk(l, prices);
    for (int i = 0; i < l; i++) {
      cout << low[i];
      if (i < l - 1)
        cout << " ";
    }
    cout << endl;

    // Print out h highest prices
    reverse(prices.begin(), prices.end());
    vector<int> high = getk(h, prices);
    reverse(high.begin(), high.end());
    for (int i = 0; i < h; i++) {
      cout << high[i];
      if (i < h - 1)
        cout << " ";
    }
    cout << endl;
  }

  return 0;
}
