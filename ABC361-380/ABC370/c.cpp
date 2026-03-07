#include <bits/stdc++.h>
#include <atcoder/all>
#define rep(i, s, n) for (int i = s; i < (n); i++)
#define ALL(a) a.begin(),a.end()
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
using namespace std;
using namespace atcoder;
using ll = long long;
using pii = pair<int,int>;
using vi = vector<int>;
using vll = vector<ll>;
using vpii = vector<pii>;
using vvi = vector<vi>;
using vvl = vector<vll>;

int main()
{
    string S,T;
    cin >> S >> T;
    int s_size = S.size();
    vector<string> X;
    rep(i,0,s_size){
        if(S[i]<=T[i]){
            continue;
        }else if(S[i]>T[i]){
            S[i] = T[i];
            string s;
            rep(j,0,s_size){
                s.push_back(S[j]);
            }
            X.push_back(s);
        }
    }
    rep(i,0,s_size){
        if(S[s_size-1-i]>=T[s_size-1-i]){
            continue;
        }else{
            S[s_size-1-i] = T[s_size-1-i];
            string s;
            rep(j,0,s_size){
                s.push_back(S[s_size-1-j]);
            }
            reverse(ALL(s));
            X.push_back(s);
        }
    }
    cout << X.size() << endl;
    rep(i,0,X.size()){
        cout << X[i] << endl;
    }
}