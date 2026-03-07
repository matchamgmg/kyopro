#include <bits/stdc++.h>
#include <atcoder/all>
#define rep(i, s, n) for (int i = s; i < (n); i++)
#define ALL(a) a.begin(),a.end()
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
using namespace std;
using namespace atcoder;
using ll = long long;
using pii = pair<int,int>;
using vi = vector<int>;
using vll = vector<ll>;
using vpii = vector<pii>;
using vvi = vector<vi>;
using vvl = vector<vll>;

int main()
{
    int N;
    cin >> N;
    vvi A(N,vi(N));
    rep(i,0,N){
        rep(j,0,i+1){
            cin >> A[i][j];
        }
    }
    int a = 1;
    rep(i,0,N){
        if(a>=(i+1)){
            a = A[a-1][i];
        }else{
            a = A[i][a-1];
        }
    }
    cout << a << endl;
}