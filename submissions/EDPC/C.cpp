#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

void chmax(int &a,int b){
    if(a<b) a = b;
}

int main()
{
    int N;
    cin >> N;
    int a,b,c;
    vector<int> dp_a(N+1,0),dp_b(N+1,0),dp_c(N+1,0);
    //最後に行った活動でわける
    rep(i,N){
        cin >> a >> b >> c;
        chmax(dp_a[i+1],dp_b[i]+a);
        chmax(dp_a[i+1],dp_c[i]+a);
        chmax(dp_b[i+1],dp_a[i]+b);
        chmax(dp_b[i+1],dp_c[i]+b);
        chmax(dp_c[i+1],dp_a[i]+c);
        chmax(dp_c[i+1],dp_b[i]+c);
    }
    cout << max({dp_a[N],dp_b[N],dp_c[N]}) << endl;
}