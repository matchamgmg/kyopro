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
using vvll = vector<vll>;

int main()
{
    int N;
    cin >> N;
    vll A(N),B(N);
    rep(i,0,N){
        cin >> A[i];
    }
    rep(i,0,N){
        cin >> B[i];
    }
    sort(ALL(A));
    sort(ALL(B));
    cout << (A.back()+B.back()) << endl;
}