#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int N,Q;
    cin >> N;
    vector<int> A(N);
    rep(i,N) cin >> A[i];
    cin >> Q;
    vector<int> B(Q);
    rep(i,Q) cin >> B[i];

    sort(ALL(A));
    /*
    sort(ALL(B));
    int i=0,j=0;
    for(;i<Q;i++){
        while(j<N-1 && B[i]>A[j]) j++;
        if(j==N) cout << abs(B[i]-A[N-1]) << endl;
        else if(j==0) cout << abs(B[i]-A[j]) << endl;
        else cout << min(abs(B[i]-A[j]),abs(B[i]-A[j-1])) << endl;
    }
    */
    //Bの順番を崩さないためには尺取り法ではなく二分探索を用いる．

    //lower_bound
    rep(i,Q){
        int index = lower_bound(ALL(A),B[i]) - A.begin();
        if(index==N){
            cout << abs(B[i]-A[index-1]) << endl;
        }
        else if(index==0){
            cout << abs(B[i]-A[index]) << endl;
        }
        else{
            cout << min(abs(B[i]-A[index]),abs(B[i]-A[index-1])) << endl;
        }
    }
}