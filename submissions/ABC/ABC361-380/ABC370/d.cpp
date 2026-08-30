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
using vvl = vector<vll>;

int main()
{
    int H,W;
    cin >> H >> W;
    int Q;
    cin >> Q;
    ll ans = (ll)H * (ll)W;
    vector<vector<bool>> wall(H,vector<bool>(W,true));
    //破壊した壁の座標を記録
    vector<set<int>> yoko(H,set<int>());
    vector<set<int>> tate(W,set<int>());
    
    rep(i,0,Q){
        int r,c;
        cin >> r >> c;
        r--;
        c--;
        //nibutann
        if(wall[r][c]){
            wall[r][c] = false;
            yoko[r].insert(c);
            tate[c].insert(r);
            ans--;
            continue;
        }
        //migi
    }
    cout << ans << endl;
}