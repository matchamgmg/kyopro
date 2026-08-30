#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

//10^4 * 10^5
//s_max > 10*5

int main()
{
    int H,W;
    cin >> H >> W;
    vector<vector<int>> A(H,vector<int>(W)), B(H,vector<int>(W));
    rep(i,H){
        rep(j,W){
            cin >> A[i][j];
        }
    }
    rep(i,H){
        rep(j,W){
            cin >> B[i][j];
        }
    }
    ll cnt = 0;
    rep(i,H-1){
        rep(j,W-1){
            int s = B[i][j]-A[i][j];
            A[i][j] += s;
            A[i+1][j] += s;
            A[i][j+1] += s;
            A[i+1][j+1] += s;
            cnt += abs(s);
        }
    }
    rep(i,H){
        rep(j,W){
            if(A[i][j]!=B[i][j]){
                cout << "No" << endl;
                return 0;
            }
        }
    }
    cout << "Yes" << endl;
    cout << cnt << endl;
}