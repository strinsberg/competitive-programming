#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'

string O,H;

map<char, int> values{
  {'A',14}, {'K',13}, {'Q',12}, {'J',11}, {'T',10},
  {'9',9}, {'8',8},{'7',7},{'6',6},{'5',5},{'4',4},
  {'3',3},{'2',2}
};


int points(int i, int j) {
  int d = values[H[i]] - values[O[j]];
  if (d == 0)
    return 1;
  if (d > 0)
    return 2;
  return 0;
}

int points_change(int i, int j) {
  int p1,p2,sp1,sp2;
  p1 = points(i,i);
  p2 = points(j,j);
  sp1 = points(i,j);
  sp2 = points(j,i);
  return (sp1+sp2) - (p1+p2);
}

void bubble() {
  for (int i = 0; i < (int)H.size() - 1; i++) {
    cout << O << endl << H << endl;
    for (int j = i+1; j < (int)H.size(); j++) {
      //cout << i << "," << j << "   " << points_change(i,j) << endl;
      if (points_change(i, j) >= 0) {
        char t = H[i];
        H[i] = H[j];
        H[j] = t;
      }
    }
  }
}


int solve() {
  int ans = 0;
  for (int i = 0; i < (int)H.size(); i++)
    ans += points(i,i);
  return ans;
}

bool comp(char a, char b) {
  return values[a] < values[b];
}

int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  int n; cin >> n;

  while (n--) {
    cin >> O >> H;
    vector<int> V(H.size());


    sort(O.begin(), O.end(), comp);
    sort(H.begin(), H.end(), comp);

    bubble();
    cout << endl << solve() << endl;

    cout << O << endl << H << endl;
  }
  return 0;
}
