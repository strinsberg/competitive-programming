#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'

string input;
ll i;
vector<ll> STACK;

void N() {
  i++;
  if (input[i] == 'S') {
    if (STACK.size() == 0)
      cout << "Invalid copy operation" << endl;
    else
      STACK.push_back(STACK.back());
  } else if (input[i] == 'T') {
     if (STACK.size() < 2)
       cout << "Invalid swap operation" << endl;
     else {
       ll n = STACK.back();
       STACK.back() = STACK[STACK.size() - 2];
       STACK[STACK.size() - 2] = n;
     }
  } else {
     if (STACK.size() == 0)
       cout << "Invalid remove operation" << endl;
     else
       STACK.pop_back();
  }
  i++;
}

void INT() {
  i++;
  vector<char> N;
  ll sign = (input[i++] == 'S') ? 1 : -1;
  while (input[i] != 'N')
    N.push_back(input[i++]);

  ll num = 0;
  for (ll j = (ll)N.size() - 1; j >= 0; j--) {
    ll x = (N[N.size() - 1 - j] == 'S' ? 0 : 1) << j;
    num += x;
  }
  STACK.push_back(num * sign);
  i++;
}

void S() {
  i++;
  if (input[i] == 'S') {
    INT(); 
  } else {
    N();
  }
}

void T() {
  i++;
  if (input[i] == 'S' and input[i+1] == 'S') {
    i+=2;
    if (input[i] == 'T') {
      if (STACK.size() < 2)
        cout << "Invalid subtraction operation" << endl;
      else {
        ll a = STACK.back(); STACK.pop_back();
        ll b = STACK.back(); STACK.pop_back();
        STACK.push_back(b - a);
      }
    } else if (input[i] == 'S') {
      if (STACK.size() < 2)
        cout << "Invalid addition operation" << endl;
      else {
        ll a = STACK.back(); STACK.pop_back();
        ll b = STACK.back(); STACK.pop_back();
        STACK.push_back(a + b);
      }
    } else {
      if (STACK.size() < 2)
        cout << "Invalid multiplication operation" << endl;
      else {
        ll a = STACK.back(); STACK.pop_back();
        ll b = STACK.back(); STACK.pop_back();
        STACK.push_back(a * b);
      }
    }
    i++;
  } else if (input[i] == 'S') {
    i+=3;
    if (STACK.size() < 2)
      cout << "Invalid division operation" << endl;
    else {
      ll a = STACK.back(); STACK.pop_back();
      ll b = STACK.back(); STACK.pop_back();
      if (a == 0)
        cout << "Division by zero" << endl;
      else
        STACK.push_back(b / a);
    }
  } else if (input[i] == 'N') {
    i+=3;
    if (STACK.size() == 0)
      cout << "Invalid print operation" << endl;
    else {
      cout << STACK.back() << endl;
      STACK.pop_back();
    } 
  } else {
    assert(false);
  }
}

void prog() {
  while (i < (ll)input.size()) {
    if (input[i] == 'S')
      S();
    else if (input[i] == 'T')
      T();
    else
      assert(false);
  }
}


int main() {
  //ios_base::sync_with_stdio(0); cin.tie(NULL);

  cin >> input;
  i = 0;
  prog();

  return 0;
}
