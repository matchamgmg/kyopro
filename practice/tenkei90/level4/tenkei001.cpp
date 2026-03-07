#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int N,L,K;
    cin >> N >> L >> K;
    vector<int> A(N);
    rep(i,N){
        cin >> A[i];
    }
    vector<int> B(N+1);
    B[0] = A[0];
    rep(i,N-1){
        B[i+1] = A[i+1] - A[i];
    }
    B[N] = L - A[N-1];
    /*
    rep(i,N+1){
        cout << B[i] << " ";
    }
    cout << endl;
    */
    int l = 0,r = L;
    //すべての羊羹を長さx以上にできるか、のxの最大値
    //二分探索において、T,Fの境界が一つ定まる
    //<==> l(T側の最大値),r(F側の最小値)が隣り合わせになっている
    //==> ループの継続条件は r-l>1
    while(r-l>1){//r==l+1となるとき，lは求めるxの最大値
    //cout << l << " " << r << endl;
        int mid = (l+r)/2;
        int i = 0;
        int cnt = 0;
        while(i<=N){
            int len = 0;
            while(i<N+1 && len<mid){
                len += B[i];
                i++;
            }
            if(len>=mid){
                cnt++;
                //cout << cnt << endl;
            }
        }
        if(cnt>=K+1){
            l = mid;
        }else{
            r = mid;
        }
    }
    int ans = l;
    cout << ans << endl;
}