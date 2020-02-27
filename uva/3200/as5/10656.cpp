////////////////////////////////////////////////////////////////
// CPSC 3200
// Assignment 5
// UVA 10656
//
// Steven Deutekom
// Jan 31 2020
//
// The maximum sub-sequence is always all the non-zero elements
// in the list. So we strip all the 0's and output the other numbers
// in the order they came in. We have to be careful if there are only 0's
// though and output one 0 in that case as this will be the shortest
// maximal sequence.
//
// Also, I am stupid and do not seem to know the difference between
// a sub-string/contiguous sub-sequence and a sub-sequence. But
// Josip always sets me straight.
////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;

int main() {
  while (1) {
    int n;
    cin >> n;

    // Break when n is 0
    if (!n) break;

    // Get all the elements and ignore 0's
    vector<int> ans;
    for (int i = 0; i < n; i++) {
      int k;
      cin >> k;
      if (k > 0)
        ans.push_back(k);
    }

    // If there are no numbers in the sequence it was all 0's so say 0
    // else cout all the numbers in the sequence
    int m = ans.size();
    if (!m) {
      cout << 0 << endl;
    } else {
      for (int i = 0; i < m; i++) {
        cout << ans[i];
        if (i < m - 1) cout << " ";
      }
      cout << endl;
    }
  } 

  return 0;
}
