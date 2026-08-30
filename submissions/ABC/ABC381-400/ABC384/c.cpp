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
#define next_p(a) next_permutation(all(a))
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}

int main()
{
    vi point(5);
    rep(i,0,5){
        cin >> point[i];
    }
    vector<char> ABCDE = {'A','B','C','D','E'};
    vector<string> name{"A","B","C","D","E","AB","AC","AD","AE","BC","BD","BE","CD","CE","DE"
        ,"ABC","ABD","ABE","ACD","ACE","ADE","BCD","BCE","BDE","CDE","ABCD","ABCE","ABDE","ACDE","BCDE","ABCDE"};
    vi score(31,0);
    rep(i,0,31){
        rep(j,0,name[i].size()){
            rep(k,0,5){
                if(name[i][j]==ABCDE[k]){
                    score[i] += point[k];
                }
            }
        }
    }
    vi idx(31);
    iota(all(idx),0);
    stable_sort(all(idx),[&](int i, int j){ return score[i]>score[j] || (score[i]==score[j] && name[i]<name[j]); });
    for(int i : idx){
        cout << name[i] << endl;
    }
}