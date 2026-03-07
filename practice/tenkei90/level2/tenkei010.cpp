#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int N;
    cin >> N;
    vector<int> P1(N+1,0),P2(N+1,0);
    int c,p;
    rep(i,N){
        cin >> c >> p;
        if(c==1){
            P1.at(i+1) = P1.at(i) + p;
            P2.at(i+1) = P2.at(i);
        }else if(c==2){
            P1.at(i+1) = P1.at(i);
            P2.at(i+1) = P2.at(i) + p;
        }
    }
    int Q;
    cin >> Q;
    int l,r;
    rep(i,Q){
        cin >> l >> r;
        cout << P1.at(r) - P1.at(l-1) << " " << P2.at(r) - P2.at(l-1) << endl;
    }
}