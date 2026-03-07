## LIS (最長増加部分列)
数列$A$が与えられたとき，$A$の（連続とは限らない）部分列であって，狭義単調増加となるものの中で，長さが最大となるものを，$A$の最長増加部分列という
## LISの求め方
### $O(N^2)$ 解法　愚直DP
$A=\{A_1, A_2, \dots, A_N\}$について，$dp[i]=$ "$A_i$を最終要素に持つ増加部分列の最大長"とする．

遷移方法

* $dp[i+1] = \max \{dp[i+1], 1\}$
* $dp[i+1] = \max \{dp[j+1]+1 \,|\, A[j]<A[i]\}$

答え

$\max_{0\leq i\leq N} \{dp[i]\}$

コード
```C++
#include<bits/stdc++.h>
using namespace std;

int main(){
    int N;
    cin >> N;
    vector<int> A(N);
    for(int i=0; i<N; i++) cin >> A[i];
    vector<int> dp(N+1,0);

    for(int i=0; i<N; i++){
        dp[i+1] = max(dp[i+1], 1);
        for(int j=0; j<i; j++){
            if(A[i]>A[j]){
                dp[i+1] = max(dp[i+1], dp[j+1]+1);
            }
        }
    }

    cout << *max_element(dp.begin(), dp.end()) << endl;
}
```
### 計算量改善 $O(N\log N)$
最終要素が$A_i$より小さい増加部分列を良い部分列と呼ぶことにする

最長の良い部分列の末尾に$A_i$を加えるのが最適

$A_1, A_2, ...,A_{i-1}$における増加部分列であって，長さが$x$のものを考えると，その中で最終要素が最小のものの末尾に$A_i$を加えるのが最適

$L[x]:=$ "$A_1, A_2, \dots , A_{i-1}$において，長さが$x$であるような増加部分列の最終要素としてあり得る値の最小値"

$L[x]$は定義より広義単調増加であるから，$A_i$を末尾に加えることのできる増加部分列のうち最小のものの長さは"$L[x] < A_i$であるような$x$の最大値"であり，二分探索により求められる

$L$の更新

$L[dp[i+1]] = A[i]$

コード
```C++
#include<bits/stdc++.h>
using namespace std;

int main(){
    int N; cin >> N;
    vector<int> A(N);
    for(int i=0; i<N; i++) cin >> A[i];

    const int INF = 1<<29;
    vector<int> L(N+1,INF);
    L[0] = -INF;
    vector<int> dp(N+1,0);

    for(int i=0; i<N; i++){
        auto itr = lower_bound(L.begin(), L.end(), A[i]);
        int k = itr - L.begin();
        dp[i+1] = k;
        L[dp[i+1]] = A[i];
    }
    
    cout << *max_element(dp.begin(), dp.end()) << endl;
}
```
LISの長さは$\infty$でない$L$の要素のインデックスの最大値に等しい

よって管理する配列はひとつでよい
```C++
    int N; cin >> N;
    vector<int> A(N);
    for(int i=0; i<N; i++) cin >> A[i];

    const int INF = 1<<30;
    vector<int> L(N+1,INF);
    L[0] = -INF;

    int ans = 0;
    for(int i=0; i<N; i++){
        auto itr = lower_bound(L.begin(), L.end(), A[i]);
        int k = itr - L.begin();
        L[k] = A[i];
        ans = max(ans, k);
    }
    
    cout << ans << endl;
```