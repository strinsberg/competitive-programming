#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'



int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  vector<int> nums(4);
  for (int i = 0; i < 4; i++)
    cin >> nums[i];

  sort(nums.begin(), nums.end());

  int a, b, c;
  a = nums[3] - nums[0];
  b = nums[3] - nums[1];
  c = nums[3] - nums[2];

  cout << a << " " << b << " " << c << endl;

  return 0;
}
