#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int N;
    cin >> N;
    for(int i=0;i<(1<<N);i++){
        string s = "";
        int cnt = 0;
        for(int j=0;j<N;j++){
            if((i>>(N-1-j)) & 1 ){
                s.push_back(')');
                cnt--;
                if(cnt<0) break;
            }else{
                s.push_back('(');
                cnt++;
            }
        }
        if(cnt == 0) cout << s << endl;
    }
}