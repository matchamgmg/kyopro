#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)

int main()
{
    int n,x;
    cin >> n >> x;
    vector<int> A(n-1);
    int sum = 0;
    rep(i,n-1){
        cin >> A.at(i);
    }
    rep(i,101){
        sum = 0;
        vector<int> B(n,0);
        rep(i,n-1){
            B.at(i)=A.at(i);
        }
        B.back() = i;
        sort(B.begin(),B.end());
        rep(i,n-2){
            sum += B.at(i+1);
        }
        if(sum>=x){
            cout << i << endl;
            return 0;
        }
    }
    cout << -1 << endl;
}