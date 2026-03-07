#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()

int main()
{
    int n,m;
    cin >> n >> m;
    vector<int> A(n);
    rep(i,n){
        cin >> A.at(i);
    }
    sort(ALL(A));
    int ans = 0,cnt;
    auto itr = begin(A);
    rep(i,n){
        auto itr2 = lower_bound(ALL(A),A.at(i)+m);
        cnt = distance(itr,itr2);
        ans = max(ans,cnt);
        advance(itr,1);
    }
    cout << ans << endl;
}