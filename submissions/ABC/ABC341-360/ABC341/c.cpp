#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)

int main()
{
    int H,W,N;
    cin >> H >> W >> N;
    string T;
    cin >> T;
    vector<string> S(H);
    rep(i,H){
        cin >> S.at(i);
    }

    auto check = [&](int xx,int yy){
        if(S.at(yy).at(xx)=='#'){
            return false;
        }else{
            return true;
        }
    };

    int x,y;
    bool a;
    int cnt = 0;
    for(int i=1;i<H-1;i++){
        for(int j=1;j<W-1;j++){
            y=i;
            x=j;
            a=true;
            if(!check(x,y)){
                a=false;
            }
            rep(k,N){
                if(T.at(k)=='L'){
                    x--;
                }else if(T.at(k)=='R'){
                    x++;
                }else if(T.at(k)=='U'){
                    y--;
                }else{
                    y++;
                }
                if(!a||!check(x,y)){
                    a=false;
                    break;
                }
            }
            if(a){
                cnt++;
            }
        }
    }
    cout << cnt << endl;
}