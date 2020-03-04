///////////////////////////////////////////////////////////////////////
// CPSC 3200
// Project
// rmc18 Problem H -> Small Schedule
//
// Steven Deutekom
// March 2 2020
//
// This problem can be done greedily. We do all the long jobs that we can
// on our machines. Then we do all small jobs.
// We do not have to simulate it because we know how many times we will have
// to use all our machines for long jobs and small jobs. So we can just
// multiply this number by the time for the jobs.
// Must make sure that if we are not using all the machines to do large jobs
// that we are keeping track of how many small jobs we could do on those
// machines at the same time.
///////////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;

#define ll long long


int main() {
  int q, m, s, l;
  cin >> q >> m >> s >> l;

  ll ans = 0;

  // First run all longer jobs
  if (l) {
    ans += (l / m) * q;

    // run any left over larger jobs and any 1 second jobs that can be
    // run at the same time
    if (l % m) {
      ans += q;
      s -= (m - (l % m)) * q;
    }
  }

  // If there are any 1 second jobs left run them
  if (s > 0) {
    ans += s / m;
    ans += (s % m) > 0;
  }

  cout << ans << endl;

  return 0;
}
