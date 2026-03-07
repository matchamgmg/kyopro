#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;
#define INF 1LL<<50

int main()
{
    ll N;
    cin >> N;
    stack<pair<ll,ll>> H;
    ll cnt = 0;
    H.push({INF,cnt});
    ll ans = 1;
    for(ll i=0;i<N;i++){
        ll h;
        cin >> h;
        while(H.top().first<=h){
            pair<ll,ll> p = H.top();
            cnt -= p.second;
            ans -= p.first * p.second;
            H.pop();
        }
        H.push({h,i+1-cnt});
        pair<ll,ll> p = H.top();
        ans += p.first * p.second;
        cnt += p.second;
        cout << ans << endl;
    }
}