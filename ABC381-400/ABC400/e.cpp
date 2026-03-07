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

// 1 <= x <= 10^6 について
// x = p^k * q^l (k,l > 0) が存在するか
// 区間篩を用いて各xについて素因数の種類数をカウント

vector<int> enum_primes(){
    vector<int> res;
    vector<bool> is_prime(1000001, true);
    is_prime[0] = is_prime[1] = false;
    for(ll i=2; i<1000001; ++i){
        if(!is_prime[i]) continue;
        res.emplace_back(i);
        for(ll j=i*i; j<1000001; j+=i){
            is_prime[j] = false;
        }
    }
    return res;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<int> prime_numbers = enum_primes();

    // 素因数の種類数だけわかればよい

    vector<int> prime_facts(1000001, 0);
    for(int p : prime_numbers){
        for(int i=p; i<1000001; i+=p){
            prime_facts[i]++;
        }
    }

    vector<ll> number;
    rep(i,0,1000001){
        if(prime_facts[i] == 2) number.emplace_back(((ll)i)*i);
    }

    int Q;
    cin >> Q;
    for(;Q--;){
        ll A;
        cin >> A;
        cout << *(prev(upper_bound(all(number),A))) << endl;
    }

}