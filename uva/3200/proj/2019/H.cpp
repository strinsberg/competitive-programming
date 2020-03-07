///////////////////////////////////////////////////////////////////////
// CPSC 3200
// Project
// rmc19 Problem H -> The Biggest Triangle
//
// Steven Deutekom
// March 4 2020
//
// Because there are only 100 lines we can just check each triple of
// lines to see if they form a triangle. If they do we get the 3
// intersection points and find the perimeter of the triangle they make.
// We can keep track of the max perimeter we have seen and if by the
// end the perimeter is 0 then there is no triangle made.
//
// Uses the intersect_iline library code to find the intersections
// between lines. I also used hypot because everything is in doubles
// and the precision is only 1e-5 and I thought it would be enough.
// And while it is accepted I can't prove that it was a good choice.
///////////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;


/* how close to call equal */
const double EPSILON = 1E-12;


struct Point {
  double x, y;
  Point() : x(0), y(0) {}
  Point(double a, double b) : x(a), y(b) {}
};


/* returns 1 if intersect at a point, 0 if not, -1 if the lines coincide */
int intersect_iline(Point a, Point b, Point c, Point d, Point &p)
{
  double r;
  double denom, num1, num2;

  assert((a.x != b.x || a.y != b.y) && (c.x != d.x || c.y != d.y));

  num1 = (a.y - c.y)*(d.x - c.x) - (a.x - c.x)*(d.y - c.y);
  num2 = (a.y - c.y)*(b.x - a.x) - (a.x - c.x)*(b.y - a.y);
  denom = (b.x - a.x)*(d.y - c.y) - (b.y - a.y)*(d.x - c.x);

  if (fabs(denom) >= EPSILON) {
    r = num1 / denom;
    p.x = a.x + r*(b.x - a.x);
    p.y = a.y + r*(b.y - a.y);
    return 1;
  } else {
    if (fabs(num1) >= EPSILON) {
      return 0;
    } else {
      return -1;
    }
  }
}

// My Code ///////////////////////////////////////////////////////////////////

// All the lines
vector<pair<Point, Point>> L;


// Get a perimeter of the triangle created by a,b,c
double get_perim(Point a, Point b, Point c) {
  double ab, bc, ca;
  ab = hypot(a.x - b.x, a.y - b.y);
  bc = hypot(b.x - c.x, b.y - c.y);
  ca = hypot(c.x - a.x, c.y - a.y);

  return ab + bc + ca;
}


// Calculate the perimeters of all possible traingles and return the
// perimeter of the largest one.
double solve() {
  double perim = 0.0;
  Point a,b,c;
  int e,f,g;

  int n = L.size();
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      e = intersect_iline(L[i].first, L[i].second, L[j].first, L[j].second, a);
      if (e != 1)  // There is no intersection
        continue;

      for (int k = j + 1; k < n; k++) {
        f = intersect_iline(L[i].first, L[i].second, L[k].first, L[k].second, b);
        g = intersect_iline(L[k].first, L[k].second, L[j].first, L[j].second, c);
        if (f != 1 or g != 1)  // There is no triangle
          continue;

        perim = max(perim, get_perim(a,b,c)); 
      }
    }
  }

  return perim;
}


int main() {
  int n;
  cin >> n;

  for (int i = 0; i < n; i++) {
    double a,b,c,d;
    cin >> a >> b >> c >> d;
    L.emplace_back(Point(a,b), Point(c,d));
  }

  double ans = solve();

  if (ans > 0)
    cout << ans << endl;
  else
    cout << "no triangle" << endl;

  return 0;
}
