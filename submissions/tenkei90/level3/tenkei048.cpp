#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int N,K;
    cin >> N >> K;
    vector<ll> A(N),B(N);
    vector<ll> C;
    rep(i,N){
        cin >> A[i] >> B[i];
        C.push_back(B[i]);
        C.push_back(A[i]-B[i]);
    }
    sort(ALL(C),greater<ll>{});
    ll score = 0;
    rep(i,K){
        score += C[i];
    }
    cout << score << endl;
}