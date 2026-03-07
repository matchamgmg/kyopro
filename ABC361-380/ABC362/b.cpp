#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int xa,ya,xb,yb,xc,yc;
    cin >> xa >> ya >> xb >> yb >> xc >> yc;
    int a,b,c;
    a = (xa-xb)*(xa-xb) + (ya-yb)*(ya-yb);
    b = (xb-xc)*(xb-xc) + (yb-yc)*(yb-yc);
    c = (xc-xa)*(xc-xa) + (yc-ya)*(yc-ya);
    if(a+b==c||b+c==a||c+a==b){
        cout << "Yes" << endl;
    }else{
        cout << "No" << endl;
    }
}