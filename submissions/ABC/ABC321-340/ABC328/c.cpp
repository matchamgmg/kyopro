#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)

int main()
{
    int n,q;
    string s;
    cin >> n >> q >> s;
    vector<int> a(n+1,0); //直前の文字が同じであるような文字の個数
    rep(i,n-1){
        int j = i+1;
        if(s.at(j)==s.at(i)){
            a.at(j+1) = a.at(j)+1;
        }else{
            a.at(j+1) = a.at(j);
        }
    }
    vector<int> l(q),r(q);
    rep(i,q){
        cin >> l.at(i) >> r.at(i);
    }
    rep(i,q){
        cout << a.at(r.at(i)) - a.at(l.at(i)) << endl;
    }
}