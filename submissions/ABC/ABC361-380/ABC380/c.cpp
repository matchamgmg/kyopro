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
    int N,K;
    cin >> N >> K;
    string s;
    cin >> s;
    int pos1,pos2;
    int cnt = 0;
    int len1 = 0,len2 = 0;
    rep(i,0,N){
        if(i==0){
            if(s[i]=='1') cnt++;
            pos1 = i;
            len1++;
        }else{
            if(s[i-1]=='0' && s[i]=='1'){
                cnt++;
                pos2 = pos1;
                len2 = len1;
                pos1 = i;
                len1 = 1;
            }else if(s[i]=='1'){
                len1++;
            }else if(cnt==K){
                break;
            }
        }
    }
    rep(i,pos1,pos1+len1){
        s[i] = '0';
    }
    rep(i,pos2+len2,pos2+len2+len1){
        s[i] = '1';
    }
    cout << s << endl;
}