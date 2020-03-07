///////////////////////////////////////////////////////////////////////
// CPSC 3200
// Project
// rmc19 Problem b -> Fantasy Draft
//
// Steven Deutekom
// March 4 2020
//
// The approach is to simulate the draft. I save
// an index in the players list and owner preference lists to avoid
// seaching them from the beginning every time. Selected players are
// placed in a set. When taking a players turn we advance the index to
// The first unpicked player before making any picks.
///////////////////////////////////////////////////////////////////////
#include<bits/stdc++.h>
using namespace std;


int n, k;
vector<vector<string>> L;  // Each owners preference list
vector<string> P;          // The list of players to draft
vector<int> I;             // Position in each owner list
vector<vector<string>> T;  // Teams
set<string> S;             // Already selected players


void solve() {
  int top = 0;  // Top unchosen player
  I = vector<int>(n);
  T = vector<vector<string>>(n);

  // Simulate the draft
  // Outer loop just keeps track of the total picks made
  for (int i = 0; i < n * k; i+=n) {

    // Take each players turn in a round
    for (int j = 0; j < n; j++) {
      // Skip any players in the owners list that have been picked
      while (I[j] < (int)L[j].size() and S.count(L[j][I[j]]))
        I[j]++;

      // If the owner has no more preferences pick the next available
      // player on the list.
      if (I[j] >= (int)L[j].size()) {
        while (top < (int)P.size() and S.count(P[top]))
          top++;

        T[j].push_back(P[top]);
        S.insert(P[top++]);

      // Otherwise pick the owners next preference
      } else {
        S.insert(L[j][I[j]]);
        T[j].push_back(L[j][I[j]]);
        I[j]++;
      }
    }
  }
}


int main(){
  cin >> n >> k;

  // Get owner lists
  L = vector<vector<string>>(n);
  for (int i = 0; i < n; i++) {
    int m; cin >> m;
    for (int j = 0; j < m; j++) {
      string p; cin >> p;
      L[i].push_back(p);
    }
  }

  // Get player list
  int l; cin >> l;
  P = vector<string>(l);
  for (auto& x : P) cin >> x;

  solve();

  // Print out the teams
  for (int i = 0; i < n; i++) {
    for (auto& p : T[i])
      cout << p << " ";
    cout << endl;
  }

  return 0;
}
