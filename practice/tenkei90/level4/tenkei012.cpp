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
    int H,W,Q;
    cin >> H >> W >> Q;
    vector<vector<bool>> grid(H,vector<bool>(W,false));
    dsu union_find(H*W);
    vi dx = {1,0,-1,0};
    vi dy = {0,1,0,-1};
    rep(i,0,Q){
        int q;
        cin >> q;
        if(q==1){
            int r,c;
            cin >> r >> c;
            r--;
            c--;
            grid[r][c] = true;
            rep(j,0,4){
                if(r+dy[j]>=0&&r+dy[j]<H&&c+dx[j]>=0&&c+dx[j]<W){
                    if(grid[r+dy[j]][c+dx[j]]){
                        union_find.merge(r*W+c,(r+dy[j])*W+(c+dx[j]));
                    }
                }
            }
        }else{
            int ra,ca,rb,cb;
            cin >> ra >> ca >> rb >> cb;
            ra--;
            ca--;
            rb--;
            cb--;
            if(grid[ra][ca] && grid[rb][cb] && union_find.same(ra*W+ca,rb*W+cb)){
                pyes;
            }else{
                pno;
            }
        }
    }
}