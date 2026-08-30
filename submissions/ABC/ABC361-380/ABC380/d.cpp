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
    string s;
    cin >> s;
    int Q;
    cin >> Q;

    auto f = [&](auto f, ll m) -> pair<char,int> {
        ll siz = (ll)s.size();
        if(m<=siz){
            return {s[m-1],0};
        }
        int n = 0;
        while(siz < m){
            siz *= 2;
            n++;
        }
        n--;
        ll mm = m - siz/2;
        pair<char,int> res = f(f,mm);
        res.second++;
        return res;
    };


    rep(i,0,Q){
        ll k;
        cin >> k;
        pair<char,int> ans = f(f,k);
        if(ans.second%2 == 0){
            cout << ans.first << ' ';
        }else{
            if(isupper(ans.first)){
                cout << (char)tolower(ans.first) << ' ';
            }else{
                cout << (char)toupper(ans.first) << ' ';
            }
        }
    }
}