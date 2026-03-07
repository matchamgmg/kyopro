#include <bits/stdc++.h>
#include <atcoder/all>
#define rep(i, s, n) for (int i = s; i < (n); i++)
#define ALL(a) a.begin(),a.end()
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
using namespace std;
using namespace atcoder;
using ll = long long;
using pii = pair<int,int>;
using vi = vector<int>;
using vll = vector<ll>;
using vpii = vector<pii>;
using vvi = vector<vi>;
using vvll = vector<vll>;
using Graph = vvi;
using wGraph = vector<vpii>;

long double calc(ll x1,ll y1,ll x2,ll y2){
    return sqrt((long double)((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));
}

int main()
{
    int N;
    cin >> N;
    ll x1 = 0,y1 = 0;
    ll x2 = 0,y2 = 0;
    long double ans = 0;
    rep(i,0,N){
        cin >> x2 >> y2;
        ans += calc(x1,y1,x2,y2);
        x1 = x2;
        y1 = y2;
    }
    x2 = 0;
    y2 = 0;
    ans += calc(x1,y1,x2,y2);
    printf("%.12Lf\n",ans);
}