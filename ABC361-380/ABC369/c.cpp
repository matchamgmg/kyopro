#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

ll cmb(ll cnt){
    if(cnt==1) return 0;
    return cnt*(cnt-1)/2;
}

int main()
{
    ll N;
    cin >> N;
    vector<ll> A(N);
    rep(i,N){
        cin >> A[i];
    }
    //kaisa
    vector<ll> kaisa(N-1);
    rep(i,N-1){
        kaisa[i] = A[i+1] - A[i];
        //cout << kaisa[i] << " ";
    }
    //cout << endl;
    ll prev;
    ll cnt = 1;
    ll ans = 0;
    rep(i,N-1){
        if(i==0){
            prev = kaisa[i];
            cnt = 1;
            if(N==2){
                ans += cmb(cnt+1);
            }
        }else if(kaisa[i]==prev){
            cnt++;
            //cout << cnt << endl;
            if(i==N-2){
                ans += cmb(cnt+1);
            }
        }else{
            prev = kaisa[i];
            //cout << cmb(cnt+1) << endl;
            ans += cmb(cnt+1);
            //cout << cnt << endl;
            cnt = 1;
            if(i==N-2){
                ans += cmb(cnt+1);
            }
        }
    }
    ans += N;
    cout << ans << endl;
}