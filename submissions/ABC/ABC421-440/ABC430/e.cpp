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

using ull = unsigned long long;
const ull MASK30 = (1ULL << 30) - 1;
const ull MASK31 = (1ULL << 31) - 1;
const ull MOD = (1ULL << 61) - 1;
const ull MASK61 = MOD;

const ull POSITIVIZER = MOD*4;

ull Mull(ull a, ull b){
    ull au = a >> 31;
    ull ad = a & MASK31;
    ull bu = b >> 31;
    ull bd = b & MASK31;
    ull mid = ad * bu + au *bd;
    ull midu = mid >> 30;
    ull midd = mid & MASK30;
    return au*bu*2 + midu + (midd<<31) + ad*bd;
    //return CalcMod(au*bu*2 + midu + (midd<<31) + ad*bd);
}

ull CalcMod(ull x){
    ull xu = x >> 61;
    ull xd = x & MASK61;
    ull res = xu + xd;
    if(res>=MOD) res -= MOD;
    return res;
}

ull GCD(ull a, ull b){
    if(a%b==0) return b;
    return GCD(b,a%b);
}

ull PowMod(ull base, ull exp){
    ull res = 1;
    while(exp>0){
        if(exp&1) res = CalcMod(Mull(res,base));
        exp >>= 1;
        base = CalcMod(Mull(base,base));
    }
    return res;
}

ull RandBase(ull base0, ull A, mt19937_64 &mt){
    ull res;
    ull k;
    do{
        k = mt()%(MOD-1);
        if(GCD(k,MOD-1)!=1) continue;
        res = PowMod(base0,k);
    }while(res<=A);
    return res;
}

int main()
{
    mt19937_64 mt(0);
    ull base0 = 37;
    ull base = RandBase(base0,100,mt);
    vector<ull> powMemo (1000001,0);
    powMemo[0] = 1;
    rep(i,0,1000001){
        powMemo[i+1] = CalcMod(Mull(powMemo[i],base));
    }
    int T;
    cin >> T;
    while(T--){
        string A,B;
        cin >> A >> B;
        string AA = A + A;
        int N = A.size();
        vector<ull> hashA(2*N+1,0),hashB(N+1,0);
        rep(i,0,2*N){
            hashA[i+1] = CalcMod(Mull(hashA[i],base) + AA[i]);
        }
        rep(i,0,N){
            hashB[i+1] = CalcMod(Mull(hashB[i],base) + B[i]);
        }
        int ans = -1;
        rep(i,0,N){
            ull hash2 = CalcMod(hashA[i+N] + POSITIVIZER - Mull(hashA[i],powMemo[N]));
            if (hash2 == hashB[N]){
                ans = i;
                break;
            }
        }
        cout << ans << endl;
    }
}