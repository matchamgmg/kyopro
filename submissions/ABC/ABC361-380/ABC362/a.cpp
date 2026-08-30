#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int r,g,b;
    string c;
    cin  >> r >> g >> b >> c;
    if(c=="Red"){
        cout << min(g,b) << endl;
        return 0;
    }
    if(c=="Blue"){
        cout << min(r,g) << endl;
        return 0;
    }
    if(c== "Green"){
        cout << min(r,b) << endl;
        return 0;
    }
}