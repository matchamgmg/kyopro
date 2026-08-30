#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int n,m,k;
    cin >> n >> m >> k;
    vector<int> C(m);
    vector<set<int>> A(m,set<int>());
    vector<string> R(m);
    int a;
    rep(i,m){
        cin >> C.at(i);
        rep(j,C.at(i)){
            cin >> a;
            A.at(i).insert(a);
        }
        cin >> R.at(i);
    }
    vector<int> bits(n); //先頭から1,2,...N
    int ans = 0;
    bool c = true;
    for(int i=0;i<(1<<n);i++){
        for(int j=0;j<n;j++){
            int div = (1<<j);
            bits.at(j) = (i/div)%2;
        }
        vector<bool> check(m,false);
        rep(j,m){
            int cnt = 0;
            rep(k,n){
                if(bits.at(k)!=1){
                    continue;
                }
                if(A.at(j).count(k+1)){
                    cnt++;
                }
            }
            if(cnt>=k&&R.at(j).at(0)=='o'){
                check.at(j) = true;
            }else if(cnt<k&&R.at(j).at(0)=='x'){
                check.at(j) = true;
            }
        }
        c = true;
        rep(j,m){
            c &= check.at(j);
        }
        if(c){
            ans++;
        }
    }
    cout << ans << endl;
}