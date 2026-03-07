#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int N;
    cin >> N;
    vector<ll> A(2*N);
    ll whole = 0;
    rep(i,N){
        cin >> A[i];
        A[N+i] = A[i];
        whole += A[i];
    }
    int i = 0,j = 0;
    ll ans = 0;
    while(true){
        while(i<2*N&&ans*10<whole){
            ans += A[i];
            i++;
        }
        while(j<2*N&&ans*10>whole){
            ans -= A[j];
            j++;
        }
        if(ans*10==whole) {
            cout << "Yes" << endl;
            return 0;
        }
        if(i==2*N&&ans*10<whole){
            break;
        }
    }
    cout << "No" << endl;
}