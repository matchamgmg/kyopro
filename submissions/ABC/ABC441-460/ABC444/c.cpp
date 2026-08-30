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
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
#define YN(a) if(a){ pyes; }else{ pno; }
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}
#define grid_check(x,y,X,Y) (0<=x && x<X && 0<=y && y<Y)
#define input(A) rep(i,0,A.size()) cin >> A[i]
template<class T>
bool chmax(T &a, T b){ if(a < b){ a = b; return true; } else return false; }
template<class T>
bool chmin(T &a, T b){ if(a > b){ a = b; return true; } else return false; }

// L 最低A_max

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    vector<ll> A(N);
    ll A_max = 0;
    rep(i,0,N){
        cin >> A[i];
        chmax(A_max, A[i]);
    }
    bool same = true;
    rep(i,0,N){
        same &= (A_max == A[i]);
    }
    vector<ll> ans;
    if(same){
        ans.emplace_back(A_max);
        if(N%2 == 0){
            ans.emplace_back(A_max*2);
        }
    }else{
        sort(all(A));
        auto f = [&](ll m) -> bool {
            int i = 0, j = N-1;
            bool res = true;
            while(j>=0 && A[j]==m) j--;
            if(j == 0) return true;
            if((j-i)%2 == 0) return false;
            while(i<j){
                res &= (A[i] + A[j] == m);
                i++;
                j--;
            }
            return res;
        };
        if(f(A_max)) ans.emplace_back(A_max);
        if(f(A_max+A[0])) ans.emplace_back(A_max+A[0]);
    }
    for(ll a : ans){
        cout << a << ' ';
    }
    cout << endl;
}