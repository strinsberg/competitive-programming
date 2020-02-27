///////////////////////////////////////////////////////////////////////
// CPSC 3200
// Assignment 1
// UVA 10420
//
// Steven Deutekom
// Jan 7 2020
//
// Takes all input lines and counts the first word using a map.
//
// To extract the first word I am extracting the whole line with
// getline. This is necessary because we are not guaranteed how many
// words are in the name of the woman. Then to get the first work I
// am using string stream to get one string from the line and I then
// ignore the rest.
//
// At the end we simply print out the country and frequency from the
// map. Because the map is ordered by key our output is already in
// alphabetical order.
///////////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;

int main() {
  int n;
  cin >> n;
  cin.ignore();

  map<string, int> count;

  for (int i = 0; i < n; i++) {
    string input;
    getline(cin, input);

    // get country name
    stringstream ss(input);
    string country;
    ss >> country;

    // add to country frequency
    if (count.find(country) == count.end())
      count[country] = 1;
    else
      count[country]++;
  }

  for (auto& c : count)
    cout << c.first << " " << c.second << endl;

  return 0;
}
