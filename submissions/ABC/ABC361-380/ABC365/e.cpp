#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int N;
    cin >> N;
    vector<int> A(N);
    rep(i,N){
        cin >> A[i];
    }
    vector<int> xorA(N);
    xorA[0] = A[0];
    for(int i=0;i<(int)A.size()-1;i++){
        xorA[i+1] = xorA[i] ^ A[i+1];
    }
    cout << endl;
    rep(i,N){
        cout << xorA[i] << " ";
    }
    cout << endl;
    vector<int> xorB(N-1);
    xorB[0] = xorA.back();
    for(int i = 0;i<(int)xorB.size()-1;i++){
        if((N-1-i)%2==0){
            xorB[i+1] = xorB[i]; 
        }else{
            xorB[i+1] = xorB[i] ^ A[i];
        }
    }
    ll ans = 0;
    rep(i,xorB.size()){
        ans += xorB[i];
    }
    cout << ans << endl;
}