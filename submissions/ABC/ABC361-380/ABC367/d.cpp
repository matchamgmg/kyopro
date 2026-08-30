#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int N,M;
    cin >> N >> M;
    vector<ll> A(2*N);
    rep(i,N){
        cin >> A[i];
        A[i+N] = A[i];
    }
    vector<ll> B(2*N+1,0);
    map<int,int> m;
    ll cnt = 0;
    rep(i,2*N){
        B[i+1] = B[i] + A[i];
        if(i>=N){
            m[B[i-N+1]%M]--;
            cnt += m[B[i-N+1]%M];
        }
        m[B[i+1]%M]++;
        //cout << (B[i+1]%M) << " " << m[B[i+1]%M] << endl;
    }
    cout << cnt << endl;

}