#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int N,M;
    cin >> N >> M;
    vector<ll> A(N),B(M);
    rep(i,N){
        cin >> A[i];
    }
    rep(i,M){
        cin >> B[i];
    }
    sort(ALL(A));
    sort(ALL(B));
    int j = 0;
    ll cost = 0;
    rep(i,M){
        int cnt = 0;
        while(j<N&&A[j]<B[i]){
            cnt++;
            j++;
        }
        if(i!=0&&cnt==0){
            j++;
        }
        if(j>=N){
            cout << -1 << endl;
            return 0;
        }
        cost += A[j];
    }
    cout << cost << endl;
}