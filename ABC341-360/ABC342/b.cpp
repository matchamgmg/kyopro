#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)

int main()
{
    int N;
    cin >> N;
    vector<int> P(N);
    int p;
    rep(i,N){
        cin >> p;
        P.at(p-1)=i+1;
    }
    int Q;
    cin >> Q;
    int a,b;
    rep(i,Q){
        cin >> a >> b;
        cout << (P.at(a-1)<P.at(b-1)?a:b) << endl;
    }
}