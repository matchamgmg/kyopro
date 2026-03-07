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
    rep(i,N){
        cin >> A[i];
    }
    int cnt = 0;
    while(true){
        int check = 0;
        sort(ALL(A),greater<int>{});
        A[0]--;
        A[1]--;
        cnt++;
        rep(i,N){
            if(A[i]>0){
                check++;
            }
        }
        if(check<=1) break;
    }
    cout << cnt << endl;
}