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
    vector<char> S(N);
    int ans = 0;
    int a;
    char s;
    int posi_l;
    int posi_r;
    bool left = true,right = true;
    rep(i,N){
        cin >> a >> s;
        if(left && s=='L'){
            posi_l = a;
            left = false;
        }
        if(right && s=='R'){
            posi_r = a;
            right = false;
        }
        if(!left && s=='L'){
            ans += abs(posi_l-a);
            posi_l = a;
        }
        if(!right && s=='R'){
            ans += abs(posi_r-a);
            posi_r = a;
        }
    }
    cout << ans << endl;

}