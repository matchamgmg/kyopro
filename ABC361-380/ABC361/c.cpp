#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int N,K;
    cin >> N >> K;
    vector<int> A(N);
    rep(i,N){
        cin >> A[i];
    }
    sort(ALL(A));
    int min_sub = 1<<30;
    for(int i=0;i+N-K-1<N;i++){
        min_sub = min(min_sub,A[i+N-K-1]-A[i]);
    }
    cout << min_sub << endl;
}