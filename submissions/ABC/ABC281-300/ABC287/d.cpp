#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
using vi = vector<int>;
using vll = vector<ll>;
using vld = vector<ld>;
using vmint = vector<mint>;
using vvi = vector<vi>;
using vvll = vector<vll>;
using vvld = vector<vld>;
using vvmint = vector<vmint>;
using vvvi = vector<vvi>;
using vvvll = vector<vvll>;
using vvvld = vector<vvld>;
using vvvmint = vector<vvmint>;
template<class T> using pq = priority_queue<T, vector<T>>;//大きい順
template<class T> using pq_g = priority_queue<T, vector<T>, greater<T>>;//小さい順

#define rep(i, s, n) for (int i = s; i < (n); i++)
#define rrep(i, s, n) for (int i = (n) - 1; i >= s; i--)
#define all(a) a.begin(),a.end()
#define rall(a) a.rbegin(),a.rend()
#define chmax(x,y) x = max(x,y)
#define chmin(x,y) x = min(x,y)
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
#define next_p(a) next_permutation(all(a))
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n)rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}

int main()
{
    string S,T;
    cin >> S >> T;
    int s = S.size(), t = T.size();
    int not_match_back = -1;
    rep(i,0,t){
        if(S[s-1-i]=='?' || T[t-1-i]=='?') continue;
        if(S[s-1-i]!=T[t-1-i]){
            not_match_back = s-1-i;// S'にこの文字が含まれていたらNG
            break;
        }
    }
    int not_match_front = s;
    rep(i,0,t){
        if(S[i]=='?' || T[i]=='?') continue;
        if(S[i]!=T[i]){
            not_match_front = i;// S'にこの文字が含まれていたらNG
            break;
        }
    }
    rep(i,0,t+1){
        if(s-t+i <= not_match_back || i-1 >= not_match_front){
            pno;
        }else{
            pyes;
        }
    }
}