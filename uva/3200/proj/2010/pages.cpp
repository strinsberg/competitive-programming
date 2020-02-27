#include <bits/stdc++.h>
using namespace std;

// Remove all commas from a string
string fix(string str) {
  for (int i = 0; i < (int)str.size(); i++) {
    if (str[i] == ',')
      str[i] = ' ';
  }
  return str;
}

// Add all pages in a valid range to the set of pages
void getPages(int len, string str, set<int>& pages) {
  int s, e;
  char d;
  
  // Put the str in string stream for parsing and make sure start > 1
  stringstream ss(str);
  ss >> s;
  s = s > 0 ? s : 1;

  // If there is a range get the second number
  // or set start = end
  if (str.find('-') != string::npos) {
    ss >> d;
    ss >> e;
  } else {
    e = s;
  }
  
  // Make sure end is valid
  if (e < s) return;
  e = e <= len ? e : len;

  // Add all pages in range to the set
  for (;s <= e; s++) {
    pages.insert(s);
  }
}

int main() {
  int n;

  while (cin >> n && n) {
    // Get the page ranges and remove commas
    string line;
    cin.ignore();
    getline(cin, line);
    stringstream ranges(fix(line));

    // For each range put the pages it covers in the pages set
    string str;
    set<int> pages;
    while (ranges >> str) {
      getPages(n, str, pages);
    }

    // output the number of pages to print
    cout << pages.size() << endl;
  }
  return 0;
}
