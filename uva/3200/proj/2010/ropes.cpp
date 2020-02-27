#include <bits/stdc++.h>
using namespace std;

// How many climbers can make it up a pitch with a rope
int climbers(int r, int p) {
  int c = (r / p) + 1;
  return c > 1 ? c : 0;
}

// Is a rope long enough
bool down(int r, int sum) {
  return r / 2 >= sum;
}

int main() {
  int n, _50, _60, _70;

  while (cin >> n && n) {
    _50 = _60 = _70 = 1e9;
    int sum = 0;
    
    // Check how many climbers can get up for each pitch
    for (int i = 0; i < n; i++) {
      int p;
      cin >> p;
      _50 = min(climbers(50, p), _50);
      _60 = min(climbers(60, p), _60);
      _70 = min(climbers(70, p), _70);
      sum+=p;
    }
    
    // Make sure the rope is long enough to get down
    _50 = down(50, sum) ? _50 : 0;
    _60 = down(60, sum) ? _60 : 0;
    _70 = down(70, sum) ? _70 : 0;   

    cout << _50 << " " << _60 << " " << _70 << endl;
  } 

  return 0;
}
