#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)
#define waru 100000000

int func(int x,int y){
    return (x+y)%(waru);
}

int main()
{
    int N;
    cin >> N;
    vector<int> A(N);
    rep(i,N){
        cin >> A.at(i);
    }
    sort(A.begin(),A.end());
    int count =0;
    int a;
    rep(i,N){

        a = (int)(A.end() - lower_bound(A.begin(),A.end(),waru-A.at(i)));
        if(A.at(i)>=waru/2){
            count += a - 1;//自分自身は数えない
        }else if(a>0){
            count += a;
        }
    
    }
    count/=2;
    int64_t sum = 0;
    rep(i,N){
        sum += (int64_t)A.at(i);
    }
    int64_t ans = (int64_t)(N-1) * sum;
    ans -= (int64_t)count * waru;
    cout << ans << endl;
}