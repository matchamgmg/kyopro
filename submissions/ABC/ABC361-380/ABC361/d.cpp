#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int search(string s,string t){
    int min_root = 1<<30;
    int idx = -1;
    rep(i,s.size()+2){
        if(s[i]=='.') idx = i;
    }
    rep(i,s.size()-1){
        string ss = s;
        if(ss[i]!='.'&&ss[i+1]!='.'){
            swap(ss[i],ss[idx]);
            swap(ss[i+1],ss[idx+1]);
            min_root = min(min_root,search(ss,t));
        }
    }
}

int main()
{
    int N;
    cin >> N;
    string S,T;
    cin >> S >> T;
    int cnts = 0;
    int cntt = 0;
    rep(i,N){
        if(S[i]=='B') cnts++;
        if(T[i]=='B') cntt++;
    }
    if(cnts!=cntt){
        cout << -1 << endl;
        return 0;
    }
    S += "..";
    T += "..";
}