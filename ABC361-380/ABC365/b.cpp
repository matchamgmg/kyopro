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
    int max_idx = -1;
    int max_num = -1;
    int sec_idx = -1;
    int sec_num = -1;
    rep(i,N){
        cin >> A[i];
        if(sec_num < A[i]){
            if(max_num < A[i]){
                sec_num = max_num;
                sec_idx = max_idx;
                max_num = A[i];
                max_idx = i;
            }else{
                sec_num = A[i];
                sec_idx = i;
            }
            
        }
    }
    cout << (sec_idx+1) << endl;
}