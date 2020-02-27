#include <bits/stdc++.h>
using namespace std;

typedef unsigned long long ull;

// Should turn it to a ull right away and keep it that way. The only reason I did all the string
// converting was because I thought I was shifting bytes and not bits.

string rol3(string str) {
  ull n = stoull(str, nullptr, 16);
  stringstream ss;
  ss << setfill('0') << setw(16) << setbase(16) << (n << 3 | n >> 61);
  return ss.str();
}

string ror8(string str) {
  ull n = stoull(str, nullptr, 16);
  stringstream ss;
  ss << setfill('0') << setw(16) << setbase(16) << (n >> 8 | n << 56);
  return ss.str();
}

pair<string,string> R(string x, string y, ull ki) {
  ull xp = (stoull(ror8(x), nullptr, 16) + stoull(y, nullptr, 16)) ^ ki;
  ull yp = stoull(rol3(y), nullptr, 16) ^ xp;

  stringstream sx,sy;
  sx << setfill('0') << setw(16) << setbase(16) << xp;
  sy << setfill('0') << setw(16) << setbase(16) << yp;

  return {sx.str(),sy.str()};
}

// L2 = Lk[0], L1 = Lk[1], L0 = Lk[2], k = Lk[3]
vector<string> speck_ks(vector<string> Lk, ull i) {
  pair<string, string> l2k = R(Lk[2], Lk[3], i);
  Lk[2] = Lk[1];
  Lk[1] = Lk[0];
  Lk[0] = l2k.first;
  Lk[3] = l2k.second;
  return Lk;
}

pair<string,string> speck(pair<string, string> xy, vector<string> Lk) {
  for (ull i = 0; i < 34; i++) {
    //cout << Lk[0] << " " << Lk[1] << " " << Lk[2] << " " << Lk[3] << endl;
    //cout << xy.first << " " << xy.second << endl;

    xy = R(xy.first, xy.second, stoull(Lk[3], nullptr, 16));
    Lk = speck_ks(Lk, i);
  }
  return xy;
}

int main() {
  string key, text;

  while (cin >> key >> text and key != "end") {
    pair<string, string> xy;
    vector<string> Lk;

    Lk.push_back(key.substr(0, 16));
    Lk.push_back(key.substr(16, 16));
    Lk.push_back(key.substr(32, 16));
    Lk.push_back(key.substr(48, 16));
    
    xy.first = text.substr(0,16);
    xy.second = text.substr(16, 16);

    pair<string, string> xyp = speck(xy, Lk);
    cout << xyp.first << xyp.second << endl;
  }

  return 0;
}
