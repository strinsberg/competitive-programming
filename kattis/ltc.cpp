#include <bits/stdc++.h>
using namespace std;

map<string, string> ns;

pair<string,size_t> sub(string&, size_t);
string eval(string&);

pair<string, size_t> replace(string& line, size_t start = 1) {
  string fixed;
  size_t i;
  for (i = start; i < line.size(); i++) {
    char ch = line[i];

    if (ch == '`') {
      break;
    } else if (ch == '$' and line[i+1] == '{') {
      auto subbed = sub(line, i);
      fixed.append(subbed.first);
      i = subbed.second;
    } else {
      fixed.push_back(ch);
    }
  }

  return make_pair(fixed, i);
}

pair<string,size_t> sub(string& line, size_t start) {
  string subbed;
  size_t i;
  for (i = start + 2; i < line.size(); i++) {
    char ch = line[i];

    if (ch == '}') {
      break;
    } else if (ch == '`') {
      auto repd = replace(line, i + 1);
      subbed.append(repd.first);
      i = repd.second;
    } else if (ch == '\"') {
      size_t pos = line.find('\"', i + 1);
      string expr = line.substr(i, pos - i + 1);
      subbed = eval(expr);
      i = pos + 1;
      break;
    } else {
      subbed.push_back(ch);
    }
  }

  if (ns.find(subbed) == ns.end())
    return make_pair(subbed, i);

  return make_pair(ns[subbed], i);
}

string eval(string& expr) {
  if (expr[0] == '\"') {
    return expr.substr(1, expr.size() - 2);
  } else if (expr[0] == '`') {
    return replace(expr).first;
  } else {
    return ns[expr];
  }
}

int main() {
  while (1) {
    string op, var("empty"), val, _;
    
    cin >> op;
    if (op == "end.") break;
    
    if (op == "var") {
      cin >> var >> _;
      cin.ignore();
      getline(cin, val);
      val.pop_back();
      string res = eval(val);
      ns[var] = res;
    } else {
      cin.ignore();
      getline(cin, val);
      val.pop_back();
      cout << eval(val) << endl;
    }
  }

  return 0;
}
