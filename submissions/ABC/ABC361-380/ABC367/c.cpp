#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

void search(int d,int n,int k,vector<int> &r,vector<int> &ans){
    if(d==n){
        if(accumulate(ALL(ans),0)%k==0){
            rep(i,ans.size()){
                cout << ans[i] << " ";
            }
            cout << endl;
        }
    }else{
        rep(i,r[d]){
            ans[d] = i+1;
            search(d+1,n,k,r,ans);
        }
    }
}

int main()
{
    int N,K;
    cin >> N >> K;
    vector<int> R(N);
    rep(i,N){
        cin >> R[i];
    }
    vector<int> A(N);
    search(0,N,K,R,A);
}