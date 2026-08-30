#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    ll N, X, Y;
    cin >> N >> X >> Y;
    vector<ll> A(N), B(N);
    rep(i,N){
        cin >> A[i];
    }
    rep(i,N){
        cin >> B[i];
    }
    sort(ALL(A),greater<ll>{});
    sort(ALL(B),greater<ll>{});
    ll suma = 0, sumb = 0;
    ll cnt = 0;
    rep(i,N){
        suma += A[i];
        sumb += B[i];
        if(suma>X||sumb>Y){
            cnt = i+1;
            cout << cnt << endl;
            return 0;
        }
    }
    cout << N << endl;
}