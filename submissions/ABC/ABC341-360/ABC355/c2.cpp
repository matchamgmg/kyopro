#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)

int main()
{
    int n,t;
    cin>>n>>t;
    vector<int> tate(n,0),yoko(n,0);
    int naname1 = 0, naname2 = 0;
    int s;
    int x,y;
    rep(i,t){
        cin>>s;
        x=(s-1)%n;
        y=(s-1)/n;
        tate.at(x)++;
        if(tate.at(x)==n){
            cout<<i+1<<endl;
            return 0;
        }
        yoko.at(y)++;
        if(yoko.at(y)==n){
            cout<<i+1<<endl;
            return 0;
        }
        if(x==y){
            naname1++;
            if(naname1==n){
                cout<<i+1<<endl;
                return 0;
            }
        }
        if(x+y==n-1){
            naname2++;
            if(naname2==n){
                cout<<i+1<<endl;
                return 0;
            }
        }
    }
    cout << -1 << endl;
}