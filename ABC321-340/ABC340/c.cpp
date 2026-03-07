#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)
using ull = unsigned long long;

int main()
{
    ull N;
    cin >> N;
    ull ans = N;
    ull a,b;
    
    for(ull i=1;;i++){
        a = N >> i;
        b = (ull)1 << i;
        //cout << a << " " << b << endl;
        if(a>1){
            ans += N;
        }else{
            ans += (N-b)*2;
            break;
        }
    }
    cout << ans << endl;
}
/*
10                  10%2==0
5 5                 10%4==2
2 3 2 3             10%8==2
1 1 1 2 1 1 1 2
1 1 1 1 1 1 1 1 1 1

9                   9%2==1
4 5                 9%4==1
2 2 2 3             9%8==1
1 1 1 1 1 1 1 2
1 1 1 1 1 1 1 1 1

23
11 12
5 6 6 6
2 3 3 3 3 3 3 3 
1 1 1 2 1 2 1 2 1 2 1 2 1 2 1 2 23%16==7

1000
500 500
250 250 250 250
125 x8
62 63
31 32
15 16
7 8
3 4
1 2

15
7 8
3 4 4 4
1 2 2 2 2 2 2 2
*/