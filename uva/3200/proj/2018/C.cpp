///////////////////////////////////////////////////////////////////////
// CPSC 3200
// Project
// rmc18 Problem C -> Forest for the Trees
//
// Steven Deutekom
// March 2 2020
//
// The most important thing to note is that this cannot be simulated.
// We only really need to check 2 points. The point closest to us and the
// point that comes right after the open area. If the first point is not in
// the open area then the only way to see belle is if she is on that point.
// If the first point is in the area then the next tree we will see is the
// first point after the open area. If belle is on this point or if she is
// in the box then we can see her. Otherwise we cannot and we print out
// the coordinates of the tree, either the first point or the one right
// after the open area.
///////////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;

#define ll long long


// Variables
ll bx, by, X1, Y1, X2, Y2;
ll g, rise, run;
pair<ll,ll> next_point;


ll gcd(ll a, ll b) {
  if (!b) return a;
  return gcd(b, a % b);
}


// Is a point in the box
bool in_box(ll x, ll y) {
  return x >= X1 and x <= X2 and y >= Y1 and y <= Y2;
}


// Get the next point that is past the box
pair<ll,ll> get_next(ll rise, ll run) {
  ll m = min(X2 / run, Y2 / rise) + 1;
  return {run * m, rise * m};
}


// Decide wether belle can be seen or not
void solve() {
  string ans = "No";
  ll ax = next_point.first, ay = next_point.second;

  // Determine if we can see belle
  if (!in_box(run, rise) and by != rise and bx != run)
    ax = run, ay = rise;
  else if (in_box(bx, by) or (by == rise and bx == run)
             or (bx == next_point.first and by == next_point.second))
    ans = "Yes";

  // Give the answer and the coordinates of the tree if can't see belle
  cout << ans << endl;
  if (ans == "No")
    cout << ax << " " << ay << endl;
}


int main() {
  cin >> bx >> by >> X1 >> Y1 >> X2 >> Y2;

  // Calculate the the rise and run of the line between us and belle and
  // the next tree after the open area
  g = gcd(bx, by);
  rise = by / g;
  run = bx / g;
  next_point = get_next(rise, run);

  solve();

  return 0;
}
