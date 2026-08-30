#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
#define DIV 998244353
using namespace std;
using ll = long long;

int main()
{
    ll n,m;
    cin >> n >> m;
    ll ans = 0;
    bitset<60> nn = n;
    vector<int> mbit(60),nbit(60);
    rep(i,60){
        int div = (1<<i);
        mbit.at(i) = (m/div)%2;
    }
    rep(i,60){
        int div = (1<<i);
        nbit.at(i) = (m/div)%2;
    }
    rep(i,nn.size()){
        if(!nn.test(i)){
            continue;
        }
        for(int j=i+1;j<nn.size();j++){
            ans += ((1<<nn.size()-j-1)*mbit.at(j))%DIV;
        }
        for(int j=0;j<=i;j++){
            ans += ((1<<nn.size()-i)*(nbit.at(j)&mbit.at(j)))%DIV;
        }
    }
    cout << ans << endl;
}