#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int n,m;
    cin >> n >> m;
    int cnt = 0;
    int h;
    rep(i,n){
        cin >> h;
        if(m>=h){
            cnt++;
            m -= h;
        }else{
            break;
        }
    }
    cout << cnt << endl;
}