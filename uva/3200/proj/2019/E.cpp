///////////////////////////////////////////////////////////////////////
// CPSC 3200
// Project
// rmc19 Problem E -> Hogwarts
//
// Steven Deutekom
// March 6 2020
//
// Both maps for the students are DFAs. To see if the junior will always make
// it to the end when the senior makes it to the end we combine the DFAs.
// This new DFA can be searched for a state where the senior reaches the
// exit room and the junior does not. If this state is contained then the
// answer is NO. If the senior never makes it to the end then it is IMPOSSIBLE,
// Otherwise it is YES.
//
// This search is done with a DFS that looks for this
// state. If the senior ever sees the exit we will set a bool to indicate
// that it is possible. Because the DFS will return true if the senior never
// makes it to the end because we will never find a state where the senior
// is at the exit and the junior is not. So this bool tells us that rather
// than NO the answer is IMPOSSIBLE.
///////////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;

#define vec vector
#define pii pair<int,int>


int n;  // The number of rooms
bool possible = false;  // Is it possible for the Senior student to reach exit
vec<vec<int>> Sdfa, Jdfa, Vis;  // Senior and Junior dfas
vec<vec<vec<pii>>> Cdfa;  // Combined Dfa


// Combines the dfas for senior and junior students into a new dfa that will
// simulate both at the same time
void combine() {
  Cdfa = vec<vec<vec<pii>>>(n+1, vec<vec<pii>>(n+1, vec<pii>(4)));

  for (int i = 0; i <= n; i++) {  // each Sdfa state
    for (int j = 0; j <= n; j++) {  // each Jdfa state
      for (int k = 0; k < 4; k++) {  // each door to take
        int a = Sdfa[i][k];
        int b = Jdfa[j][k];
        Cdfa[i][j][k] = {a,b};
      }
    }
  }
}


// DFS the combined dfa. I is state in Sdfa and J is state in Jdfa.
// If we find a state where Sdfa is at exit and Jdfa is not return false
// as this proves that Sdfa has paths not accepted in Jdfa.
// Keeps going when Jdfa is 0 just to find if Sdfa will still make it or not
int solve(int i, int j) {
  bool good = true;
  for (int k = 0; k < 4; k++) {
    int a = Cdfa[i][j][k].first;
    int b = Cdfa[i][j][k].second;

    if (!Vis[a][b]) {
      if (a == n) {  // Sfda is at exit
        possible = true;
        if (b != n)  // Jdfa is not also at exit
          return false;
      }

      Vis[a][b] = 1;
      good &= solve(a, b);
    }
  }
  return good;
}


int main() {
  // Input and initialization
  cin >> n;
  Vis = vec<vec<int>>(n+1, vec<int>(n+1));

  Sdfa = vector<vector<int>>(n+1, vector<int>(4));  
  for (int i = 1; i <= n; i++) {
    for (int j = 0; j < 4; j++) {
      cin >> Sdfa[i][j];
    }
  }

  Jdfa = vector<vector<int>>(n+1, vector<int>(4));  
  for (int i = 1; i <= n; i++) {
    for (int j = 0; j < 4; j++) {
      cin >> Jdfa[i][j];
    }
  }

  // Solve the problem
  combine();
  int ans = solve(1,1);

  // Print the ans
  if (!possible)
    cout << "Impossible" << endl;
  else if (ans)
    cout << "Yes" << endl;  
  else
    cout << "No" << endl;
}
