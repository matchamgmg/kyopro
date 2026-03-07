#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int Q;
    cin >> Q;
    int q,x;
    map<int,int> m;
    int cnt = 0;
    rep(i,Q){
        cin >> q;
        if(q==1){
            cin >> x;
            m[x]++;
            if(m[x]==1){
                cnt++;
            }
        }else if(q==2){
            cin >> x;
            m[x]--;
            if(m[x]==0){
                cnt--;
            }
        }else if(q==3){
            cout << cnt << endl;
        }
    }
}