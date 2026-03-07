#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    ll N,T;
    string S;
    cin >> N >> T >> S;
    vector<ll> X(N);
    rep(i,N){
        cin >> X[i];
    }
    vector<int> right,left;
    rep(i,N){
        if(S[i]=='1') right.push_back(X[i]);
        else left.push_back(X[i]);
    }
    sort(ALL(right));
    sort(ALL(left));
    ll cnt = 0;
    ll r = 0,l = 0,l2 = 0;
    ll rsiz = right.size();
    ll lsiz = left.size();
    for(;r<rsiz;r++){
        while(l<lsiz && left[l]<=right[r]+T*2) l++;
        while(l2<lsiz && right[r]>left[l2]) l2++;
        cnt += l;
        cnt -= l2;
    }
    cout << cnt << endl;
}