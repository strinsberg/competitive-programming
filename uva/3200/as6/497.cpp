////////////////////////////////////////////////////////////////
// CPSC 3200
// Assignment 6
// UVA 497
//
// Steven Deutekom
// Feb 22 2020
//
// Finds the longest increasing subsequence of altitudes with the
// n^2 algorithm. Stores the path length in D, and position of the
// previous altitude in the sequence in P.
//
// Reconstructing the path is done in reverse order so it is added
// to a path vector and sorted before printing.
////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'


vector<int> A,D,P,path;
int d,p;


void solve() {
    int n = A.size();
    d = 1, p = 0;
    D = vector<int>(n, 1);
    P = vector<int>(n, -1);

    // Find the best path looking at entries behind current altitude
    for (int i = 1; i < n; i++) {
      int b = 1;
      for (int j = i - 1; j >= 0; j--) {
        if (A[i] > A[j] and D[j] + 1 > b) {
          D[i] = b = D[j] + 1;
          P[i] = j;
        }
      }
      // If this path is longest update max length and position
      if (D[i] > d) {
        d = D[i];
        p = i;
      }
    }

    // Recover the path
    path = vector<int>();
    while (p >= 0) {
      path.push_back(A[p]);
      p = P[p];
    }
    sort(path.begin(), path.end());
}


int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int t; cin >> t; cin.ignore(); cin.ignore();
  while(t--) {
    // Vectors for altitudes
    A = vector<int>();

    // Get altitude input
    while (1) {
      string line;
      getline(cin, line);
      stringstream ss(line);
      
      int a;
      if (ss >> a)
        A.push_back(a);
      else
        break;
    }

    solve();

    // Print the answer
    cout << "Max hits: " << d << endl;
    for (auto a : path)
      cout << a << endl;

    if (t > 0) cout << endl;
  }

  return 0;
}
