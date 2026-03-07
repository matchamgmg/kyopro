#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)
#define waru 100000000
int main()
{
    int N;
    cin >> N;
    vector<int> A(N);
    rep(i,N){
        cin >> A.at(i);
    }
    sort(A.begin(),A.end());

    int64_t ans = 0;
    rep(i,N){
        ans += A.at(i)*(int64_t)(N-1);
    }

    int i=0;
    int r=N-1;
    for(;i<N;i++){
        while(r>=0&&A.at(i)+A.at(r)>=waru){
            r--;
        }
        ans -= (int64_t)((N-1)-max(r,i))*waru;
        //1 2 3 4 5 6 7 8 ,waru=8
        //N-1=7
        //i=2,r=3,4
        //i=4,r=1,6
        //r<iとなったときはr<x<=iを二重にカウントしないようにする
        //.r..i...
        //cout << N-1-max(r,i) << endl;
    }
    cout << ans << endl;
}