#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int N;
    cin >> N;
    vector<int> A(N);
    vector<int> W(N);
    vector<int> vec_max(N,0);
    rep(i,N){
        cin >> A[i];
    }
    int sum_weight = 0;
    rep(i,N){
        cin >> W[i];
        sum_weight += W[i];
    }
    rep(i,N){
        vec_max[A[i]-1] = max(vec_max[A[i]-1],W[i]);
    }
    int sub = accumulate(ALL(vec_max),0);
    int ans = sum_weight - sub;
    cout << ans << endl;
}