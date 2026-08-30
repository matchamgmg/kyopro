#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)
using ll = long long;

int main()
{
    int N;
    cin >> N;
    vector<ll> A(N);
    rep(i,N){
        cin >> A.at(i);
    }
    vector<int> S(N-1),T(N-1);
    rep(i,N-1){
        cin >> S.at(i) >> T.at(i);
    }
    ll a;
    rep(i,N-1){
       a = (A.at(i)/S.at(i));
       A.at(i) -= a*S.at(i);
       A.at(i+1) += a*T.at(i);
    }
    cout << A.back() << endl;
}