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
    int N;
    cin >> N;
    vll X(N+2),P(N);
    X[0] = -2000000000;
    X[N+1] = 2000000000;
    rep(i,0,N){
        cin >> X[i+1];
    }
    rep(i,0,N){
        cin >> P[i];
    }
    vll hito(N+1,0);
    rep(i,0,N){
        hito[i+1] = hito[i] + P[i];
        //cout << hito[i+1] << " ";
    }
    //cout << endl;
    int Q;
    cin >> Q;
    rep(i,0,Q){
        ll L,R;
        cin >> L >> R;
        int idx_L,idx_R;
        //nibutan
        int l = 0;
        int r = N+1;
        while(r-l>1){
            int mid = (r+l)/2;
            if(X[mid]>=L){
                r = mid;
            }else{
                l = mid;
            }
        }
        //cout << "  " << r << "  ";
        if(r==N+1){
            cout << 0 << endl;
            continue;
        }else{
            idx_L = r;
        }
        l = 0;
        r = N+1;
        while(r-l>1){
            int mid = (r+l)/2;
            if(X[mid]<=R){
                l = mid;
            }else{
                r = mid;
            }
        }
        //cout << "  " << l << "  ";
        if(l==0){
            cout << 0 << endl;
            continue;
        }else{
            idx_R = l;
        }
        //1~N
        cout << (hito[idx_R]-hito[idx_L-1]) << endl;
    }
}