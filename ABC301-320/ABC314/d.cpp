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
    int N;
    cin >> N;
    string S;
    cin >> S;
    int Q;
    cin >> Q;
    vi last(2,-1); // last[0] := type 2
    vector<pair<char,int>> A(N,{'.',-1});
    rep(i,0,Q){
        int t,x;
        char c;
        cin >> t >> x >> c;
        if(t==1){
            A[x-1] = {c,i};
            S[x-1] = c;
        }else if(t==2){
            last[0] = i;
        }else{
            last[1] = i;
        }
    }
    if(last[0]==last[1]){
        cout << S << endl;
        return 0;
    }
    rep(i,0,N){
        if(A[i].first!='.' && last[0]<A[i].second && last[1]<A[i].second){
            cout << A[i].first;
        }else{
            if((S[i]-'a'>=0 && S[i]-'a'<26)^(last[0]<last[1])){
                cout << S[i];
            }else{
                cout << (char)(S[i]^32);
            }
        }
    }
    cout << endl;
}