#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)

int main()
{
    int n,M;
    string s;
    cin >>n>>M>>s;
    int m = M;
    int ans = 0;
    int a = ans;
    rep(i,n){
        if(s.at(i)=='0'){
            m=M;
            a=ans;
        }else if(s.at(i)=='1'){
            if(m>0){
                m--;
            }else if(a>0){
                a--;
            }else{
                ans++;
            }
        }else if(s.at(i)=='2'){
            if(a>0){
                a--;
            }else{
                ans++;
            }
        }
    }
    cout << ans << endl;
}