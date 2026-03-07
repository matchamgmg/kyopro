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
    if(a==0 || b==0){
        return max(a,b);
    }
    if(a%b==0) return b;
    return gcd(b,a%b);
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    using P = pair<ll,ll>;
    P takahashi, aoki;
    cin >> takahashi.first >> takahashi.second >> aoki.first >> aoki.second;
    ll N, M, L;
    cin >> N >> M >> L;
    using D = pair<char,ll>;
    vector<D> S(M), T(L);
    rep(i,0,M) cin >> S[i].first >> S[i].second;
    rep(i,0,L) cin >> T[i].first >> T[i].second;

    map<char,int> dx, dy;
    dx['U'] = -1, dx['D'] = 1, dx['L'] = 0, dx['R'] = 0;
    dy['U'] = 0, dy['D'] = 0, dy['L'] = -1, dy['R'] = 1; 
    int i = 0, j = 0;
    ll t = 0;
    P tpos = takahashi, apos = aoki;
    ll ans = 0;
    while(t<N){
        ll dt = min(S[i].second,T[j].second);
        P dtpos = make_pair(dx[S[i].first]*dt,dy[S[i].first]*dt);
        P dapos = make_pair(dx[T[j].first]*dt,dy[T[j].first]*dt);
        P ntpos = make_pair(tpos.first+dtpos.first,tpos.second+dtpos.second);
        P napos = make_pair(apos.first+dapos.first,apos.second+dapos.second);

        if((dtpos.first==dapos.first)&&(dtpos.second==dapos.second)){
            if((tpos.first==apos.first)&&(tpos.second==apos.second)){
                ans += dt;
            }
        }else{
            if(dtpos.first==dapos.first){
                if(tpos.first==apos.first){
                    ll X = tpos.second - apos.second;
                    ll x = dy[S[i].first]-dy[T[j].first];
                    if(X%x==0){
                        X = -X/x;
                        if(0<X && X<=dt) ans++;
                    }
                }
            }else if(dtpos.second==dapos.second){
                if(tpos.second==apos.second){
                    ll X = tpos.first - apos.first;
                    ll x = dx[S[i].first]-dx[T[j].first];
                    if(X%x==0){
                        X = -X/x;
                        if(0<X && X<=dt) ans++;
                    }
                }
            }else{
                ll X = tpos.second - apos.second;
                ll x = dy[S[i].first]-dy[T[j].first];
                ll Y = tpos.first - apos.first;
                ll y = dx[S[i].first]-dx[T[j].first];
                if((X%x==0) && (Y%y==0)){
                    X = -X/x;
                    Y = -Y/y;
                    if((X==Y) && 0<X && X<=dt){
                        ans++;
                    }
                }
            }
        }


        S[i].second -= dt;
        if(S[i].second<=0) i++;
        T[j].second -= dt;
        if(T[j].second<=0) j++;
        t += dt;
        tpos = ntpos;
        apos = napos;
    }
    cout << ans << endl;
}