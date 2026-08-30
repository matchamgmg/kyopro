#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)

int main()
{
    int N,Q;
    string S;
    cin >> N >> S >> Q;
    string s = "abcdefghijklmnopqrstuvwxyz";
    char c,d;
    rep(i,Q){
        cin >> c >> d;
        rep(j,26){
            if(s.at(j)==c){
                s.at(j)=d;
            }
        }
    }
    rep(i,N){
        S.at(i)=s.at(S.at(i)-'a');
    }
    cout << S << endl;
}