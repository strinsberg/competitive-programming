///////////////////////////////////////////////////////////////////////
// CSSC 3200
// Sroject
// rmc19 Sroblem I -> Tired Terry
//
// Steven Deutekom
// March 2 2020
//
// We use partial sums to find how much sleep terry has gotten in an interval.
// However, because the pattern repeats we will insert some chars from
// the end of the pattern before the beginning. For each interval in the
// pattern we check to see how much sleep terry has gotten and compare
// it to see if he is tired. If he is tired we add to the answer.
///////////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;


int main() {
  int n, p, d, k;
  cin >> n >> p >> d;

  // Get sleep pattern and add 0 - p + 1 elements from the back to the front
  string S; cin >> S;
  k = abs(1 - p);
  S = S.substr(n - k, k) + S;

  // Create partial sum to find how much terry has slept for in any interval
  vector<int> psum(S.size() + 1);
  psum[0] = 0;
  for (size_t i = 1; i < psum.size(); i++)
    psum[i] = psum[i-1] + (S[i - 1] == 'W' ? 0 : 1);

  // Calculate how tired terry is
  int ans = 0;
  for (size_t i = k + 1; i < psum.size(); i++)
    ans += (psum[i] - psum[i - k - 1]) < d;

  cout << ans << endl;

  return 0;
}

