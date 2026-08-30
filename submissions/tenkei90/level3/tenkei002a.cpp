#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int N;
    cin >> N;
    vector<string> ss;
    for(int i=0;i<(1<<N);i++){
        vector<int> bits(N,0);
        rep(j,N){
            int div = 1<<j;
            bits[j] = (i/div)%2;
        }
        int cnt = 0;
        string s = "";
        rep(j,N){
            if(bits[j]==1){
                cnt++;
                s.push_back('(');
            }else if(bits[j]==0){
                cnt--;
                if(cnt<0)break;
                s.push_back(')');
            }
        }
        if(cnt!=0) continue;
        ss.push_back(s);
    }
    sort(ALL(ss));
    rep(i,ss.size()){
        cout << ss.at(i) << endl;
    }
}