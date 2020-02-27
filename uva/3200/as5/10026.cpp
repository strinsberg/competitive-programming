////////////////////////////////////////////////////////////////
// CPSC 3200
// Assignment 5
// UVA 10026
//
// Steven Deutekom
// Jan 31 2020
//
// This problem can be solved greedliy. A job A is strictly better than
// another job B if A.time * B.cost < B.time * A.cost. So we sort with this
// as the comparator and print out the index of each job.
////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

// Struct to keep the job because I needed custom comparator and
// didn't like pair<ll, pair<ll,ll>>
struct Job{
  ll pos;
  ll time;
  ll cost;
  bool operator<(const Job& other) const {
    return time * other.cost < other.time * cost;
  }
};

// Solve it!!!!
int main() {
  int t;
  cin >> t;

  while (t--) {
    cin.ignore();
    int n;
    cin >> n;

    // read in the jobs
    vector<Job> jobs(n);
    for (ll i = 0; i < n; i++) {
      jobs[i].pos = i;
      cin >> jobs[i].time  >> jobs[i].cost;
    }

    // Sort the jobs into the cheapest order
    sort(jobs.begin(), jobs.end());

    // Print out the proper order
    for (int i = 0; i < n; i++) {
      cout << jobs[i].pos + 1;
      if (i < n - 1) cout << " ";
    }
    cout << endl;
    if (t > 0) cout << endl;
  }
  
  return 0;
}
