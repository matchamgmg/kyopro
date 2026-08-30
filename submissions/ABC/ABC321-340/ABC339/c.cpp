#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)
using ll = long long;

int main()
{
    int N;
    cin >> N;
    ll a;

    ll bus = LONG_LONG_MAX;

    ll sum=0;
    rep(i,N){
        cin >> a;
        sum += a;
        bus = min(bus,sum);
    }
    if(bus<0){
        cout << abs(bus) + sum << endl; 
    }else{
        cout << sum << endl;
    }
}