#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

void create(int n,vector<vector<string>> &ss){
    if(ss.at(n-1).size()!=0){
        return;
    }
    if(n==1){
        ss.at(n-1) = (vector<string>(1,"()"));
        return;
    }
    create(n-1,ss);
    vector<string> vs;
    set<string> sss;
    for(auto s:ss.at(n-2)){
        sss.insert("("+s+")");
    }
    rep(i,n-1){
        if(n-2-i<i) break;
        if(n-2-i==i){
            rep(j,ss.at(i).size()){
                rep(k,ss.at(i).size()){
                    sss.insert(ss.at(i).at(j)+ss.at(i).at(k));
                    sss.insert(ss.at(i).at(k)+ss.at(i).at(j));
                }
            }
        }else{
            rep(j,ss.at(i).size()){
                rep(k,ss.at(n-2-i).size()){
                    sss.insert(ss.at(i).at(j)+ss.at(n-2-i).at(k));
                    sss.insert(ss.at(n-2-i).at(k)+ss.at(i).at(j));
                }
            }
        }
    }
    for(auto s:sss){
        vs.push_back(s);
    }
    ss.at(n-1) = vs;
}

vector<string> retvs(int n,vector<vector<string>> &ss){
    create(n,ss);
    return ss.at(n-1);
}

int main()
{
    int N;
    cin >> N;
    if(N%2==1) return 0;
    N /=2;

    vector<vector<string>> ss(N,vector<string>());
    vector<string> vs = retvs(N,ss);
    for(auto s:vs){
        cout << s << endl;
    }
}