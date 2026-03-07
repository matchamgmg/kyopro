#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int N;
    cin >> N;
    vector<int> A(2*N);
    rep(i,2*N){
        cin >> A[i];
    }
    int cnt = 0;
    rep(i,2*N-2){
        if(A[i+2]==A[i]) cnt++;
    }
    cout << cnt << endl;
}