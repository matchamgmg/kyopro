#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int N,M;
    cin >> N >> M;
    vector<string> ss(N,string());
    rep(i,N){
        cin >> ss.at(i);
    }
    int min_num = 1<<20;
    for(int i=0;i<(1<<N);i++){
        vector<int> bits(N);
        vector<int> store(M,0);
        rep(j,N){
            int div = 1<<j;
            bits[j] = (i/div)%2;
            if(bits[j]==1){
                rep(k,M){
                    if(ss.at(j).at(k)=='o'){
                        store[k] = 1;
                    }
                }
            }
        }
        if(accumulate(ALL(store),0)==M){
            min_num = min(min_num,accumulate(ALL(bits),0));
        }
    }
    cout << min_num << endl;
}