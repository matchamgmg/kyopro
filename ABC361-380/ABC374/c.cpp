#include <bits/stdc++.h>
#include <atcoder/all>
#define rep(i, s, n) for (int i = s; i < (n); i++)
#define ALL(a) a.begin(),a.end()
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
using namespace std;
using namespace atcoder;
using ll = long long;
using pii = pair<int,int>;
using vi = vector<int>;
using vll = vector<ll>;
using vpii = vector<pii>;
using vvi = vector<vi>;
using vvll = vector<vll>;

int main()
{
    int N;
    cin >> N;
    vi K(N);
    rep(i,0,N){
        cin >> K[i];
    }
    //bit全探索
    int ans = 20*100000000 + 1;
    for(int i=0;i<(1<<N);i++){
        int A = 0;
        int B = 0;
        //bit
        vector<int> bits(N);
        for(int j=0;j<N;j++){
            int div = (1<<j);
            bits.at(j) = (i/div)%2;
        }

        for(int j=0;j<N;j++){
            if(bits[j]==1){
                A += K[j];
            }else{
                B += K[j];
            }
        }
        ans = min(ans,max(A,B));
    }
    cout << ans << endl;
}