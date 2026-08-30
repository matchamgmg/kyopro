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

vector<int> MP_algorithm(string &S){
    int N = S.size();
    vector<int> border(N+1,-1);
    int j = -1;
    for(int i=0;i<N;i++){
        while(j>=0 && S[i]!=S[j]){// ここがtrueになる回数は全体で高々N回
            j = border[j];
        }
        j++;
        border[i+1] = j;
    }
    return border;
}

vector<int> KMP_algorithm(string &S){
    int N = S.size();
    vector<int> border(N+1,-1);
    int j = -1;
    for(int i=0;i<N;i++){
        while(j>=0 && S[i]!=S[j]){// ここがtrueになる回数は全体で高々O(logN)回
            j = border[j];
        }
        j++;
        if(S[i+1]==S[j]) border[i+1] = border[j];
        else border[i+1] = j;
    }
    return border;
}

int main()
{
    string S;
    cin >> S;
    string S2;
    rrep(i,0,S.size()) S2.push_back(S[i]);
    S2 += '.';
    S2 += S;
    vector<int> suffix = MP_algorithm(S2);
    int len = suffix.back();
    cout << S;
    rrep(i,0,S.size()-len){
        cout << S[i];
    }
    cout << endl;
}