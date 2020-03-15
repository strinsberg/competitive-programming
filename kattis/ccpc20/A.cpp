#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'

int n,l,h;


int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  cin >> n >> l >> h;
  cout << ((n % (l + h)) < l ? "Barb" : "Alex") << endl;

  return 0;
}
