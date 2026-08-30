#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    long double T,L,X,Y;
    cin >> T >> L >> X >> Y;
    int Q;
    cin >> Q;
    long double E;
    long double theta;
    rep(i,Q){
        cin >> E;
        long double y,z;
        y = -L/2*sin(E/T*2*M_PI);
        z = L/2*(1-cos(E/T*2*M_PI));
        long double l = sqrt(X*X+(Y-y)*(Y-y));
        theta = atan(z/l);
        printf("%.12Lf\n",theta*180/M_PI);
    }
}