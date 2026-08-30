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

// 座標圧縮だとTLE,MLEしてしまう
// N = 5000 O(N^2logN)

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    ll a, b;
    cin >> a >> b;
    vector X(4, vector<ll>(N));
    rep(i,0,N){
        cin >> X[0][i] >> X[1][i] >> X[2][i] >> X[3][i];
    }
    vector<pair<ll,ll>> NX, NY;
    const ll INF = 1LL<<60;
    {
        vll CPR, CQS;
        rep(i,0,N){
            //CPR.emplace_back(X[0][i]-1);
            CPR.emplace_back(X[0][i]);
            //CPR.emplace_back(X[2][i]);
            CPR.emplace_back(X[2][i]+1);

            //CQS.emplace_back(X[1][i]-1);
            CQS.emplace_back(X[1][i]);
            //CQS.emplace_back(X[3][i]);
            CQS.emplace_back(X[3][i]+1);
        }
        sort(all(CPR)); sort(all(CQS));
        CPR.erase(unique(all(CPR)),CPR.end());
        CQS.erase(unique(all(CQS)),CQS.end());

        rep(i,0,CPR.size()-1){
            NX.emplace_back(CPR[i],CPR[i+1]-CPR[i]);
        }
        NX.emplace_back(CPR.back(),INF);
        rep(i,0,CPR.size()-1){
            NY.emplace_back(CQS[i],CQS[i+1]-CQS[i]);
        }
        NY.emplace_back(CQS.back(),INF);
    }

    vector<vector<ll>> B(NX.size(), vector<ll>(NY.size()));
    rep(i,0,NX.size()){
        rep(j,0,NY.size()){
            B[i][j] = NX[i].second * NY[j].second;
        }
    }
    vector<vector<ll>> C(NX.size(), vector<ll>(NY.size(), 0));
    rep(i,0,N){
        // int p = lower_bound(all(NX), make_pair(X[0][i], 0LL)) - NX.begin();
        // int q = lower_bound(all(NY), make_pair(X[1][i], 0LL)) - NY.begin();
        // int r = lower_bound(all(NX), make_pair(X[2][i], 0LL)) - NX.begin();
        // int s = lower_bound(all(NY), make_pair(X[3][i], 0LL)) - NY.begin();

        // C[p][q]++;
        // C[p][s+1]--;
        // C[r+1][q]--;
        // C[r+1][s+1]++;

        int p = lower_bound(all(NX), make_pair(X[0][i], 0LL)) - NX.begin();
        int q = lower_bound(all(NY), make_pair(X[1][i], 0LL)) - NY.begin();
        int r = lower_bound(all(NX), make_pair(X[2][i]+1, 0LL)) - NX.begin();
        int s = lower_bound(all(NY), make_pair(X[3][i]+1, 0LL)) - NY.begin();

        C[p][q]++;
        C[p][s]--;
        C[r][q]--;
        C[r][s]++;
    }

    rep(i,0,NX.size()){
        rep(j,0,NY.size()-1){
            C[i][j+1] += C[i][j];
        }
    }
    rep(i,0,NY.size()){
        rep(j,0,NX.size()-1){
            C[j+1][i] += C[j][i];
        }
    }

    map<ll,ll> mp;
    rep(i,0,NX.size()){
        rep(j,0,NY.size()){
            mp[C[i][j]] += B[i][j];
        }
    }

    auto ans = *mp.rbegin();
    cout << ans.first << endl << ans.second << endl;
}