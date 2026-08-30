#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)

int main()
{
    int n;
    cin>>n;
    vector<int> w(n),x(n);
    rep(i,n){
        cin>>w.at(i)>>x.at(i);
    }
    int sum = 0;
    int sum_max = 0;
    for(int i=0;i<24;i++){
        sum=0;
        rep(j,n){
            if((x.at(j)+i)%24>=9&&(x.at(j)+i)%24<18){
                sum+=w.at(j);
            }
        }
        sum_max = max(sum_max,sum);
    }
    cout << sum_max << endl;
}