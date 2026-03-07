#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
//using mint = modint1000000007;
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
using vb = vector<bool>;
using vvb = vector<vb>;
template<class T> using pq = priority_queue<T, vector<T>>;//大きい順
template<class T> using pq_g = priority_queue<T, vector<T>, greater<T>>;//小さい順

#define rep(i, s, n) for (int i = s; i < (int)n; i++)
#define rrep(i, s, n) for (int i = (int)n - 1; i >= s; i--)
#define all(a) a.begin(),a.end()
#define rall(a) a.rbegin(),a.rend()
#define chmax(x,y) x = max(x,y)
#define chmin(x,y) x = min(x,y)
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}
#define grid_check(x,y,X,Y) (0<=x && x<X && 0<=y && y<Y)

int main()
{
    string S;
    cin >> S;
    vector<pair<char,int>> lan;
    char last = '.';
    int cnt = 0;
    rep(i,0,S.size()){
        if(S[i] == last){
            cnt++;
        }else{
            if(last!='.'){
                lan.push_back({last,cnt});
            }
            last = S[i];
            cnt = 1;
        }
    }
    lan.push_back({last,cnt});
    string ans;
    rep(i,0,lan.size()){
        if(lan[i].first=='W' && lan[i+1].first=='A'){
            ans.push_back('A');
            rep(j,0,lan[i].second){
                ans.push_back('C');
            }
            lan[i+1].second--;
        }else{
            rep(j,0,lan[i].second){
                ans.push_back(lan[i].first);
            }
        }
    }
    cout << ans << endl;
}