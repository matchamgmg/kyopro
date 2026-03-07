#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int N,Q;
    cin >> N >> Q;
    vector<ll> A(N);
    rep(i,N){
        cin >> A[i];
    }
    ll Escore = 0;
    vector<ll> B(N-1);
    rep(i,N-1){
        B[i] = A[i+1] - A[i];
        Escore += abs(B[i]);
    }
    int L,R;
    ll V;
    rep(i,Q){
        cin >> L >> R >> V;
        if(L-1>0){
            Escore -= abs(B[L-2]);
            B[L-2] += V;
            Escore += abs(B[L-2]);
        } 
        if(R<N){
            Escore -= abs(B[R-1]);
            B[R-1] -= V;
            Escore += abs(B[R-1]);
        }
        cout << Escore << endl;
    }
}