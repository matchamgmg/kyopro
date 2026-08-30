#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

ll calc(vector<ll> &A, ll m){
    ll sum = 0;
    for(int i = 0;i<(int)A.size();i++){
        sum += min(m,A[i]);
    }
    return sum;
}

int main()
{
    ll N,M;
    cin >> N >> M;
    vector<ll> A(N);
    ll sumA = 0;
    ll max_A = 0;
    rep(i,N){
        cin >> A[i];
        sumA += A[i];
        max_A = max(max_A,A[i]);
    }
    if(sumA <= M){
        cout << "infinite" << endl;
        return 0;
    }
    ll r = max_A;
    ll l = 0;
    while(r - l > 1){
        ll mid = l + (r - 1) / 2;
        if(calc(A,mid)<=M){
            l = mid;
        }else{
            r = mid;
        }
    }
    cout << l << endl;
}