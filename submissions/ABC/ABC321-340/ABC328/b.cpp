#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)

int main()
{
    int n;
    cin>>n;
    vector<int> D(n);
    int cnt=0;
    rep(i,n){
        cin>>D.at(i);
    }
    bool flag=true,flag2=true;
    rep(i,n){
        string s = to_string(i+1);
        flag=true;
        rep(j,s.size()){
            if(s.at(j)!=s.at(0)){
                flag = false;
                break;
            }
        }
        if(!flag) continue;
        rep(j,D.at(i)){
            string t = to_string(j+1);
            flag2=true;
            rep(k,t.size()){
                if(t.at(k)!=s.at(0)){
                    flag2=false;
                    break;
                }
            }
            if(flag2){
                cnt++;
            }
        }
    }
    cout << cnt << endl;
}