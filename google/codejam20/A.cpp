#include <bits/stdc++.h>
using namespace std;

int main() {
    int t; cin >> t;
    int C = 1;
    while (t--) {
        int n; cin >> n;
        
        vector<set<int>> rows(n);
        vector<set<int>> cols(n);
        vector<int> rf(n);
        vector<int> cf(n);

        int r = 0;
        int c = 0;
        long long diag = 0;
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                int x; cin >> x;
                
                if (i == j) diag += x;

                if (!rf[i] and rows[i].count(x)) {
                  r++;
                  rf[i] = 1;
                }
                if (!cf[j] and cols[j].count(x)) {
                  c++;
                  cf[j] = 1;
                }
                
                rows[i].insert(x);
                cols[j].insert(x);
            }
        }
        cout << "Case #" << C++ << ": " << diag << " " << r << " " << c << endl;
    }
    
    return 0;
}
