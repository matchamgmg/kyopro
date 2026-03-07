#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int N,A;
    cin >> N >> A;
    int time = 0;
    int t;
    rep(i,N){
        cin >> t;
        if(i==0){
            time = t + A;
        }else{
            if(time<t){
                time = t + A;
            }else{
                time = time + A;
            }
        }
        cout << time << endl;
    }
}