#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)

int main()
{
    int Q;
    cin >> Q;
    int a,b;
    vector<int> A;
    int ans;
    rep(i,Q){
        cin >> a >> b;
        if(a==1){
            A.push_back(b);
        }else{
            ans = A.at(A.size()-b);
            cout << ans << endl;
        }
    }
}