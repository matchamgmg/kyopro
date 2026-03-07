#include <bits/stdc++.h>
#include <atcoder/all>
#define rep(i, s, n) for (int i = s; i < (n); i++)
#define ALL(a) a.begin(),a.end()
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
using namespace std;
using namespace atcoder;
using ll = long long;
using pii = pair<int,int>;
using vi = vector<int>;
using vll = vector<ll>;
using vpii = vector<pii>;
using vvi = vector<vi>;
using vvll = vector<vll>;

int main()
{
    string S,T;
    cin >> S >> T;
    bool ans = true;
    rep(i,0,max((int)S.size(),(int)T.size())){
        if(i<min((int)S.size(),(int)T.size())){
            if(S[i]!=T[i]){
                ans = false;
                cout << (i+1) << endl;
                return 0;
            }
        }else{
            cout << (i+1) << endl;
            return 0;
        }
    }
    cout << 0 << endl;
}