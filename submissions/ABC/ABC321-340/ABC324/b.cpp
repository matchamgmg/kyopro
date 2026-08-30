#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)
using ll = long long;

int main()
{
    ll n;
    cin >> n;
    ll x,y;
    x = 1,y = 1;
    for(int i=0;x<=n;i++){
        x = 1;
        x = x << i;
        if(x*y==n){
            cout << "Yes" << endl;
            return 0;
        }
        for(int j=0;x*y<=n;j++){
            y *= 3;
            if(x*y==n){
                cout << "Yes" << endl;
                return 0;
            }
        }
        y = 1;
    }
    cout << "No" << endl;
}