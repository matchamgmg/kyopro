#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)
using ll = long long;

int main()
{
    ll N;
    cin >> N;
    vector<ll> x3kaibun;
    ll xxx;

    auto kaibun = [](ll x){
        string strx = to_string(x);
        int j;
        rep(i,strx.size()){
            j = strx.size()-1-i;
            if(strx.at(i)!=strx.at(j)){
                return false;
            }
        }
        return true;
    };

    for(ll i=1;;i++){
        xxx = i*i*i;
        if(xxx>N){
            break;
        }
        if(kaibun(xxx)){
            x3kaibun.push_back(xxx);
        }
    }

    cout << x3kaibun.back() << endl;
}