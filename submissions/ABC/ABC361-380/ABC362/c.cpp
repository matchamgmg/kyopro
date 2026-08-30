#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int N;
    cin >> N;
    vector<ll> L(N),R(N);
    vector<ll> sumL(N+1,0),sumR(N+1,0);
    vector<ll> X(N);
    rep(i,N){
        cin >> L[i] >> R[i];
        X[i] = L[i];
        sumL[i+1] = sumL[i] + L[i];
        sumR[i+1] = sumR[i] + R[i];
    }
    if(sumL[N]>0||sumR[N]<0){
        cout << "No" << endl;
        return 0;
    }
    cout << "Yes" << endl;
    ll sum = sumL[N];
    rep(i,N){
        if(sum < 0 && X[i] < R[i]){
            X[i] += min(R[i]-L[i],0-sum);
            sum += min(R[i]-L[i],0-sum);
        }
        cout << X[i] << " ";
    }
}