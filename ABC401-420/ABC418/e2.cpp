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
#define YN(a) if(a){ pyes; }else{ pno; }
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}
#define grid_check(x,y,X,Y) (0<=x && x<X && 0<=y && y<Y)
#define input(A) rep(i,0,A.size()) cin >> A[i]

ll gcd(ll a, ll b){
    if(a%b == 0) return b;
    return gcd(b, a%b);
}

void norm_vec(pair<ll,ll> &vec){
    if(vec.first == 0){
        vec.second = 1;
        return;
    }
    if(vec.second == 0){
        vec.first = 1;
        return;
    }
    if(vec.first < 0){
        vec.first *= -1;
        vec.second *= -1;
    }
    ll g = gcd(abs(vec.first), abs(vec.second));
    vec.first /= g;
    vec.second /= g;
    return;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 台形 : 一対の対辺が平行
    int N;
    cin >> N;
    using P = pair<ll,ll>;
    vector<P> X(N);
    rep(i,0,N) cin >> X[i].first >> X[i].second;
    vector<P> angle, center;
    rep(i,0,N-1){
        rep(j,i+1,N){
            ll x = X[j].first - X[i].first;
            ll y = X[j].second - X[i].second;
            P ang = make_pair(x,y);
            P cent = make_pair(X[i].first+X[j].first,X[i].second+X[j].second);
            norm_vec(ang);
            angle.emplace_back(ang);
            center.emplace_back(cent);
        }
    }
    sort(all(angle));
    sort(all(center));

    auto count_same = [&](ll &res, const vector<P> &vec) -> void {
        ll j = 0;
        ll M = vec.size();
        rep(i,0,M){
            while(j<M && vec[i]==vec[j]) j++;
            res += j-i-1;
        }
        return;
    };
    ll daikei = 0, heikou = 0;
    count_same(daikei, angle);
    count_same(heikou, center);

    cout << daikei - heikou << endl;
}