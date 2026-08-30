#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int N,K,X;
    cin >> N >> K >> X;
    vector<int> A(N);
    rep(i,N){
        cin >> A[i];
    }
    A.insert(A.begin() + K,X);
    rep(i,A.size()){
        cout << A[i] << " ";
    }
}