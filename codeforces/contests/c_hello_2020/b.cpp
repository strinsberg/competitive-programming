#include <bits/stdc++.h>
using namespace std;

// Descide if a sequence has an ascending subsequence
bool asc(const vector<long long>& A) {
  long long last = A[0];
  for (long long i = 1; i < A.size(); i++) {
    if (A[i] > last) return true;
    last = A[i];
  }
  return false;
}

// Solve
int main() {
  long long n;
  cin >> n;

  vector<vector<long long>> not_asc;
  for (int i = 0; i < n; i++) {
    long long l;
    cin >> l;

    vector<long long> seq(l);
    for (int j = 0; j < l; j++)
      cin >> seq[j];
    // put only non ascending sequences into collection
    if (!asc(seq)) 
      not_asc.push_back(seq);
  }

  // sort by the first element
  sort(not_asc.begin(), not_asc.end(), [](vector<long long> a, vector<long long> b) {
    return a[0] < b[0];
  });

  
  long long ans = 0;
  for (int i = 0; i < not_asc.size(); i++) {
    // for each sequence find the first sequence that has a first element
    // larger than it's back element. Then add the total number of
    // sequences before this to the ans.
    auto bound = upper_bound(not_asc.begin(), not_asc.end(), not_asc[i], [](vector<long long> a, vector<long long> b) {
      return a.back() < b.front();
    });
    long long diff = bound - not_asc.begin();
    ans += diff;
  }
  
  // We found the number of sequences that are not acsending when concatenated
  // So subtract this from the total possible
  cout << n * n - ans << endl;

  return 0;
}
