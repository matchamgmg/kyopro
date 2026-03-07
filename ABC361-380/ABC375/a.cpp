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
    int N;
    string S;
    cin >> N >> S;
    if(N<3){
        cout << 0 << endl;
        return 0;
    }
    int ans = 0;
    rep(i,0,N-2){
        if(S[i]=='#'&&S[i+1]=='.'&&S[i+2]=='#'){
            ans++;
        }
    }
    cout << ans << endl;
}