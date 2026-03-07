#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    ll sx,sy,tx,ty;
    cin >> sx >> sy >> tx >> ty;
    ll fee = 0;
    ll a = abs(sx-tx);
    ll b = abs(sy-ty);
    if(a<b){
        fee += a;
        fee += b - a;
    }else{
        fee += b;
        if((a-b)%2==0){
            fee += (a-b)/2;
        }else if((sx+sy)%2==0&&(tx+ty)%2==1){
            if(sx<tx){
                fee += (a-b)/2;
            }else{
                fee += (a-b)/2 + 1;
            }
        }else{
            if(sx<tx){
                fee += (a-b)/2 + 1;
            }else{
                fee += (a-b)/2;
            }
        }
    }
    cout << fee << endl;
}