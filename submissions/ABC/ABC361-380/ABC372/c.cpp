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
    int N,Q;
    cin >> N >> Q;
    string S;
    cin >> S;
    vi ans(Q+1,0);
    rep(i,0,N-2){
        if(S[i]=='A'&&S[i+1]=='B'&&S[i+2]=='C'){
            ans[0]++;
        }
    }
    rep(i,0,Q){
        int X;
        char C;
        cin >> X >> C;
        X--;
        bool exist = false;
        for(int Y = X-2;Y<=X;Y++){
            if(Y>=0&&Y+2<N){
            if(S[Y]=='A'&&S[Y+1]=='B'&&S[Y+2]=='C'){
                exist = true;
            }
            }
        }
        S[X] = C;
        if(exist){
            bool after = false;
            for(int Y = X-2;Y<=X;Y++){
                if(Y>=0&&Y+2<N){
                if(S[Y]=='A'&&S[Y+1]=='B'&&S[Y+2]=='C'){
                    after = true;
                }
                }
            }
            if(!after){
                ans[i+1] = ans[i]-1;
            }else{
                ans[i+1] = ans[i];
            }
        }else{
            bool after = false;
            for(int Y = X-2;Y<=X;Y++){
                if(Y>=0&&Y+2<N){
                if(S[Y]=='A'&&S[Y+1]=='B'&&S[Y+2]=='C'){
                    after = true;
                }
                }
            }
            if(!after){
                ans[i+1] = ans[i];
            }else{
                ans[i+1] = ans[i]+1;
            }
        }
        cout << ans[i+1] << endl;
    }
}