#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;
#define LARGE 1'000'000'000'000'000'000LL

ll GCD(ll a,ll b){
    while(b>0){
        a = a%b;
        swap(a,b);
    }
    return a;
}

ll LCM(ll a,ll b){
    ll d = GCD(a,b);
    ll ap = a/d;
    ll ret = 0;
    while(b>0){
        if(b&1){
            ret += ap;
            if(ret>LARGE) return -1;
        }
        ap += ap;
        b >>= 1;
    }
    return ret;
}

int main()
{
    ll A,B;
    cin >> A >> B;
    ll lcm = LCM(A,B);
    if(lcm>0) cout << lcm << endl;
    else cout << "Large" << endl;
}