#include <bits/stdc++.h>
using namespace std;

#define endl '\n'

map<string, string> ns;
size_t substitute(const string& str, string& res, size_t s);

// Parses a literal string
// should never see a quote unless the string is ending
size_t literal(const string& str, string& res, size_t s=0) {
  for (size_t i = s + 1; s < str.size(); i++) {
    if (str[i] == '\"')
      return i + 1;
    
    res.push_back(str[i]);
  }
  assert(false);
  return 0;
}

// Parses a template and replaces any ${} with their values
size_t templ(const string& str, string& res, size_t s=0) {
  for (size_t i = s + 1; s < str.size(); i++) {
    if (str[i] == '$' and str[i+1] == '{')
      i = substitute(str, res, i);
    
    if (str[i] == '`')
      return i + 1;
    
    res.push_back(str[i]);
  }
  assert(false);
  return 0;
}

// Parses a substitution ${} and replaces it with the value of the expression inside
size_t substitute(const string& str, string& res, size_t s) {
  size_t i = s + 2;
  if (str[i] == '\"')
    return literal(str, res, i) + 1;
  else if (str[i] == '`')
    return templ(str, res, i) + 1;
  else {
    string name;
    for (; i < str.size(); i++) {
      if (str[i] == '}') {
        res.append(ns[name]);
        return i + 1;
      }
      name.push_back(str[i]);
    }
  }
  assert(false);
  return 0; 
}

// Initial evaluation for expression
string eval(const string& expr) {
  string res;
  char ch = expr[0];
  if (ch == '\"')
    literal(expr, res);
  else if (ch == '`')
    templ(expr, res);
  else
    return ns[expr];

  return res;
}

// Solve the problem
int main() {
  ios_base::sync_with_stdio(0), cin.tie(0);

  while (1) {
    string op, var("empty"), val, _;
    
    // Get the input
    cin >> op;
    if (op == "end.") break;
    
    if (op == "var") {
      cin >> var >> _;
    }
    cin.ignore();
    getline(cin, val);
    val.pop_back();

    // Evaluate the value and apply the operation
    string ans = eval(val);
    if (op == "var") {
      ns[var] = ans;
    } else {
      cout << ans << endl;
    }
  }

  return 0;
}
