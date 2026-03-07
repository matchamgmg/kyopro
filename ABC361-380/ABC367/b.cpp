#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    string s;
    cin >> s;
    int cnt = 0;
    bool point = false;
    rep(i,s.size()){
        if(s[i]=='.'){
            point = true;
        }
        if(point){
            if(s[i]=='0'){
                cnt++;
            }else{
                cnt = 0;
            }
        }
    }
    if(cnt==3){
        cnt++;
    }
    rep(i,s.size()-cnt){
        cout << s[i];
    }
    cout << endl;
}