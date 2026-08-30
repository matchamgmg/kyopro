#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int N,K;
    cin >> N >> K;
    vector<int> A(N),B(N);
    rep(i,N){
        cin >> A.at(i);
    }
    rep(i,N){
        cin >> B.at(i);
    }
    int diff = 0;
    rep(i,N){
        diff += abs(A.at(i) - B.at(i));
    }
    cout << ((diff<=K&&(K-diff)%2==0)?"Yes":"No") << endl;
}