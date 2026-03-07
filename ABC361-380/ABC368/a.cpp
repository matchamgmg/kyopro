#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int N ,K;
    cin >> N >> K;
    vector<int> A(N);
    rep(i,N){
        cin >> A[i];
    }
    rep(i,N){
        cout << A[(N+i-K)%N] << " ";
    }
    cout << endl;
}