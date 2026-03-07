#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)

int main()
{
    int N;
    string Tp;
    cin >> N >> Tp;
    vector<string> S(N);
    vector<int> ans;
    rep(i,N){
        cin >> S.at(i);
    }
    int t = Tp.size();
    rep(i,N){
        if(abs((int)S.at(i).size()-t)>1){
            continue;
        }
        int k = 0;
        int cnt = 0;
        bool check = true;
        if(S.at(i).size()-t==0){
            rep(j,t){
                if(S.at(i).at(j)!=Tp.at(j)){
                    cnt++;
                }
            }
        }else if(S.at(i).size()-t==1){
            rep(j,t){
                if(S.at(i).at(k)!=Tp.at(j)){
                    if(cnt<1){
                        k++;
                        if(S.at(i).at(k)!=Tp.at(j)){
                            cnt++;
                        }
                    }
                    cnt++;
                }
                k++;
            }
        }else if(S.at(i).size()-t==-1){
            rep(j,S.at(i).size()){
                if(S.at(i).at(j)!=Tp.at(k)){
                    if(cnt<1){
                        k++;
                        if(S.at(i).at(j)!=Tp.at(k)){
                            cnt++;
                        }
                    }
                    cnt++;
                }
                k++;
            }
        }
        if(cnt<=1){
            ans.push_back(i+1);
        }
    }
    cout << ans.size() << endl;
    rep(i,ans.size()){
        cout << ans.at(i) << " ";
    }
}