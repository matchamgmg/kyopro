#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int N,M;
    cin >> N >> M;
    vector<string> ss;
    vector<set<string>> sss(N,set<string>());
    set<string> sets;
    string s;
    rep(i,N){
        cin >> s;
        ss.push_back(s);
        sets.insert(s);
    }
    sss.at(0) = sets;
    int cnt;
    rep(i,N){
        if(i==0){
            rep(j,N){
                cnt = 0;
                rep(k,M){
                    if(ss.at(j).at(k)=='o'){
                        cnt++;
                    }
                }
                if(cnt==M){
                    cout << i+1 << endl;
                    return 0;
                }
            }
        }else{
            set<string> setss;
            for(auto str:sss.at(i-1)){
                rep(j,N){
                    cnt = 0;
                    string s;
                    rep(k,M){
                        if(str.at(k)=='o'||ss.at(j).at(k)=='o'){
                            cnt++;
                            s.push_back('o');
                        }else{
                            s.push_back('x');
                        }
                    }
                    setss.insert(s);
                    if(cnt==M){
                        cout << i+1 << endl;
                        return 0;
                    }
                }
            }
            sss.at(i) = setss;
        }
    }
}