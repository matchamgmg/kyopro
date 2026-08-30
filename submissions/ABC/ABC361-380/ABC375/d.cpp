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
using Graph = vvi;
using wGraph = vector<vpii>;

int main()
{
    string S;
    cin >> S;
    int N = S.size();
    vector<map<int,int>> mae(N+1),ushiro(N+1);
    rep(i,0,N){
        map<int,int> maei,ushiroi;
        rep(l,0,26){
            if(S[i]=='A'+l){
                mae[i+1]['A'+l] = mae[i]['A'+l] + 1;
            }else{
                mae[i+1]['A'+l] = mae[i]['A'+l];
            }
            if(S[N-1-i]=='A'+l){
                ushiro[i+1]['A'+l] = ushiro[i]['A'+l] + 1;
            }else{
                ushiro[i+1]['A'+l] = ushiro[i]['A'+l];
            }
        }
    }
    ll ans = 0;
    rep(j,1,S.size()-1){
        rep(l,0,26){
            ans += (ll)mae[j]['A'+l]*ushiro[N-1-j]['A'+l];
        }
    }
    /*
    rep(i,0,N+1){
        cout << mae[i]['A'] << " ";
    }
    cout << endl;
    rep(i,0,N+1){
        cout << ushiro[i]['A'] << " ";
    }
    cout << endl;
    */
    cout << ans << endl;
}