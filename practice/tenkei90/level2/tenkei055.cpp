#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    ll n,p,q;
    cin >> n >> p >> q;
    vector<ll> A(n);
    int cnt = 0;
    rep(i,n) cin >> A[i];
    for(int i=0;i<n-4;i++){
        for(int j=i+1;j<n-3;j++){
            for(int k=j+1;k<n-2;k++){
                for(int l=k+1;l<n-1;l++){
                    for(int m=l+1;m<n;m++){
                        if(A[i]*A[j]%p*A[k]%p*A[l]%p*A[m]%p==q){
                            cnt++;
                        }
                    }
                }
            }
        }
    }
    cout << cnt << endl;
}