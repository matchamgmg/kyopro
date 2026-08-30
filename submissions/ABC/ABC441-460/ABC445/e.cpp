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

// nの最小の素因数を管理する配列
// 線形篩
vector<ll> calc_least_prime_factor(ll N){
    vector<ll> res(N+1);
    iota(all(res), 0);
    vector<ll> primes;
    for(ll i=2; i<N+1; ++i){
        if(res[i] == i) primes.emplace_back(i);
        for(ll p : primes){
            // res[i] == res[p*i]
            if(p*i>N || p>res[i]) break;
            res[p*i] = p;
        }
    }
    return res;
}

struct S {
    pair<ll,int> best, second_best;
    S() : best(0,-1), second_best(0,-1) {}
    void add(const pair<ll,int> &x){
        if(x.first >= best.first){
            second_best = best;
            best = x;
        }else if(x.first >= second_best.first){
            second_best = x;
        }
        return;
    }
};

void solve(vector<ll> &lpf){
    int N;
    cin >> N;
    vector<ll> A(N);
    ll A_max = 0;
    rep(i,0,N){
        cin >> A[i];
        chmax(A_max, A[i]);
    }
    map<ll, S> mp;
    rep(i,0,N){
        ll x = A[i];
        while(x > 1){
            ll p = lpf[x];
            ll cnt = 0;
            while(x%p==0){
                cnt++;
                x /= p;
            }
            mp[p].add(make_pair(cnt,i));
        }
    }

    mint L(1);
    for(auto p : mp){
        L *= mint(p.first).pow(p.second.best.first);
    }
    rep(i,0,N){
        mint Li = L;
        ll x = A[i];
        while(x > 1){
            ll p = lpf[x];
            while(x%p==0) x /= p;
            S s = mp[p];
            if(s.best.second == i){
                Li *= (mint(p).pow(s.best.first-s.second_best.first)).inv();
            }
        }
        cout << Li.val() << ' ';
    }
    cout << endl;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<ll> lpf = calc_least_prime_factor(1e7);
    // rep(i,0,100){
    //     cout << lpf[i] << ' ';
    // }
    // cout << endl;

    int T;
    cin >> T;
    for(;T--;) solve(lpf);
}