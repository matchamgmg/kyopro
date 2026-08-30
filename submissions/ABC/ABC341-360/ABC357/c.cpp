#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

map<int,vector<string>> m;

vector<string> process (int n){
    if(m.count(n)){
        return m[n];
    }
    vector<string> ans;
    string a;
    if(n==1){
        rep(i,3){
            if(i!=1){
                a = "";
                rep(j,3){
                    a.push_back('#');
                }
                ans.push_back(a);
            }else{
                a = "";
                rep(j,3){
                    if(j!=1){
                        a.push_back('#');
                    }else{
                        a.push_back('.');
                    }
                }
                ans.push_back(a);
            }
        }
        m[n] = ans;
        return ans;
    }else{
        vector<string> b = process(n-1);
        rep(i,3){
            if(i!=1){
                rep(j,b.size()){
                    a = "";
                    a += b.at(j);
                    a += b.at(j);
                    a += b.at(j);
                    ans.push_back(a);
                }
            }else{
                string white(b.size(),'.'); 
                rep(j,b.size()){
                    a = "";
                    a += b.at(j);
                    a += white;
                    a += b.at(j);
                    ans.push_back(a);
                }
            }
        }
        m[n] = ans;
        return ans;
    }
}

int main()
{
    int n;
    cin >> n;
    if(n==0){
        cout << '#' << endl;
        return 0;
    }
    vector<string> answer = process(n);
    rep(i,answer.size()){
        cout << answer.at(i) << endl;
    }
}