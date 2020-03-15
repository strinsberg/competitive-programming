#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'

vector<string> notes{"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
map<string, int> idxs{{"C",0}, {"C#",1}, {"D",2}, {"D#",3}, {"E",4},
                      {"F",5}, {"F#",6}, {"G",7}, {"G#",8}, {"A",9}, {"A#",10}, {"B",11}};


int dist(string& s, string& s2) {
  return (12 + (idxs[s2] - idxs[s])) % 12;
}


bool transposition(vector<string>& S, vector<string>& S2) {
  int d = dist(S[0], S2[0]);
  for (int i = 1; i < (int)S.size(); i++) {
    if (S2[i] != notes[(idxs[S[i]] + d) % 12])
      return false;
  }
  return true;
}

bool retrograde(vector<string>& S, vector<string>& S2) {
  for (int i = 0; i < (int)S.size(); i++) {
    if (S[i] != S2[(int)S.size() - 1 - i])
        return false;
  }
  return true;
}

bool inversion(vector<string>& S, vector<string>& S2) {
  if (S2[0] != S[0]) return false;
  for (int i = 1; i < (int)S.size(); i++) {
    if (S2[i] != notes[(12 + (idxs[S[0]] - dist(S[0],S[i]))) % 12])
      return false;
  }
  return true;
}

int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int n; cin >> n;
  vector<string> S(n),S2(n);
  for (auto& x : S) cin >> x;
  for (auto& x : S2) cin >> x;

  string ans("Nonsense");

  if (transposition(S,S2))
    ans = "Transposition";
  else if (retrograde(S,S2))
    ans = "Retrograde";
  else if (inversion(S,S2))
    ans = "Inversion";

  cout << ans << endl;

  return 0;
}
