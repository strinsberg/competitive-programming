////////////////////////////////////////////////////////////////
// CPSC 3200
// Assignment 4
// UVA 11516
//
// Steven Deutekom
// Jan 29 2020
//
// Sort all the houses to make sure they are in order.
// Binary search the distance answer and find the smallest one that is
// possible.
// Does this by placing the atenna and grouping all houses that are touched
// with it. Then we move to the next not touched house and put an antenna
// dist away from it and group all houses here.
//
// Josip told me how to do this, because I was trying to divide and conquer
// it by splitting the houses into groups and dividing antennas among them.
// If my approach was even possible it was too complicated the way I was doing
// it. Plus now I know how to binary search on a real value for my answer!
// And you just started talking about binary searching answers in class
// so I guess this is probably the intended solution.
/////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;

vector<int> H;
int n, m;

// Checks if this distance is a valid answer
bool check(double dist) {
  int used = 0, j;

  // Find all groups of houses that can be together under a node
  for (int i = 0; i < m; i=j) {
    used++;
    j = i;
    while (j < m and H[j] - H[i] <= 2*dist) j++;
  }

  // If used is less than number of available nodes it is possible
  return used <= n;
}


// Solve the problem
int main() {
  cout.precision(1);
  cout << fixed;

  int t;
  cin >> t;

  while (t--) {
    cin >> n >> m;

    H = vector<int>(m);
    for (int i = 0; i < m; i++)
      cin >> H[i];

    sort(H.begin(), H.end());

    // Binary search on the answer
    double mid, l = 0, h = 1e7;
    for (int i = 0; i < 100; i++) {
      mid = (l + h) / 2;
      if (check(mid))
        h = mid;
      else
        l = mid;
    }
    cout << mid << endl;
  }
}
