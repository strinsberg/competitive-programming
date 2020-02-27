///////////////////////////////////////////////////////////////////////
// CPCS 3200
// Assignment 3
// UVA 739
//
// Steven Deutekom
// Jan 10 2020
//
// Uses union find to keep track of connected computers and do fast
// Queries.
//
// Union find is Josip's. He told me not to do all the typing necessary
// to copy and paste your code!
///////////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;

// Union find data structure courtesy of Josip Smolcic
struct UF {
  int n; vector<int> A;
  UF (int n) : n(n), A(n) { iota(A.begin(), A.end(), 0); }
  int find (int a) { return a == A[a] ? a : A[a] = find(A[a]); }
  bool is_connected (int a, int b) { return find(a) == find(b); }
  void merge (int a, int b) { A[find(b)] = find(a); }
};

// Solve a set of instructions
void solve(int& success, int& failure, UF& conn) {
  while (1) {
    string line;
    getline(cin, line);
    stringstream ss(line);

    // Cases are ended by a blank line
    if (ss.str() == "") break;

    char c;
    int a, b;
    ss >> c >> a >> b;

    // For c instructions add a connection in the Union Find
    // For q instructions query the union find to see if a,b are connected
    if (c == 'c') {
      conn.merge(a,b);
    } else {
      if (conn.is_connected(a,b))
        success++;
      else
        failure++;
    }
  }
}

// Solves the problem ;)
int main() {
  int t;
  cin >> t;
  cin.ignore();

  // Take care of each test case
  for (int i = 0; i < t; i++) {
    if (i > 0)
      cout << endl;

    int n, success = 0, failure = 0;
    cin >> n;
    cin.ignore();

    // Create a union find for a test case and solve all the
    // merges and queries
    UF conn(n+1);
    solve(success, failure, conn);
    cout << success << "," << failure << endl;
  }
  return 0;
}
