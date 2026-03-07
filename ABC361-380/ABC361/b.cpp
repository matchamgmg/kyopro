#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int a,b,c,d,e,f;
    int g,h,i,j,k,l;
    cin >> a >> b >> c >> d >> e >> f;
    cin >> g >> h >> i >> j >> k >> l;
    bool ans = true;
    if(j<=a||d<=g) ans = false;
    if(k<=b||e<=h) ans = false;
    if(l<=c||f<=i) ans = false;
    
    cout << (ans?"Yes":"No") << endl;
}