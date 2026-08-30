#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)

int main()
{
    int n,t;
    cin >> n>>t;
    vector<int> a(2*n+2,0);
    int s;
    rep(i,t){
        cin >> s;
        a.at((s-1)%n) += 1;
        if(a.at((s-1)%n)==n){
            cout << i+1 << endl;
            return 0;
        }
        a.at(n + (s-1)/n) += 1;
        if(a.at(n+(s-1)/n)==n){
            cout << i+1 << endl;
            return 0;
        }
        if((s-1)%(n+1)==0){
            a.at(2*n)++;
            if(a.at(2*n)==n){
                cout << i+1 << endl;
                return 0;
            }
        }
        if(s!=1&&s!=n*n&&(s-1)%(n-1)==0){
            a.at(2*n+1)++;
            if(a.at(2*n+1)==n){
                cout << i+1 << endl;
                return 0;
            }
        }
    }
    cout << -1 << endl;
}