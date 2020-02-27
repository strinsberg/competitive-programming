///////////////////////////////////////////////////////////////////////
// CPSC 3200
// Assignment 3
// UVA 501
//
// Steven Deutekom
// Jan 11 2020
//
// Uses 2 priority queues to maintain the position of the i-min.
// We keep all numbers below i-min in below_i which is a max queue.
// We keep all numbers >= i-min in above_i which is a min queue.
//
// When a number is added we see if it is larger or smaller than the
// top in below_i. Then add it in the proper queue. If we add it in the
// below_i queue we will move the top to the above_i que. There should only
// ever be i items in the below_i que. Same when we do get we will print
// the top of the above_i que and then move it to the below_i que.
///////////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;

// Get the i-min from our black box
void get(priority_queue<int>& below_i, priority_queue<int, vector<int>, greater<int>>& above_i) {
  int x = above_i.top();
  above_i.pop();
  below_i.push(x);
  cout << x << endl;
}

// Add an element to the black box
void add(int x, priority_queue<int>& below_i, priority_queue<int, vector<int>, greater<int>>& above_i) {
  if (!below_i.empty() && x < below_i.top()) {
    above_i.push(below_i.top());
    below_i.pop();
    below_i.push(x);
  } else {
    above_i.push(x);
  }
}

// Solve the problem
int main() {
  int t;
  cin >> t;

  for (int k = 0; k < t; k++) {
    if (k > 0)
      cout << endl;
    cin.ignore();

    int M, N;
    cin >> M >> N;

    // Get the sequences
    vector<int> A(M + 1);
    for (int i = 0; i < M; i++)
      cin >> A[i+1];

    vector<int> u(N);
    for (int i = 0; i < N; i++)
      cin >> u[i];

    // Process the intructions
    priority_queue<int> below_i;
    priority_queue<int, vector<int>, greater<int>> above_i;

    int i = 1, j = 0;
    while (i + j <= M + N) {
      if (j < N && u[j] == above_i.size() + below_i.size()) {
        get(below_i, above_i);
        j++;
      } else {
        add(A[i], below_i, above_i);
        i++;
      }
    }
  }

  return 0;
}
