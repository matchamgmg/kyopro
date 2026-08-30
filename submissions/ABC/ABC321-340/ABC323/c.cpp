#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()

int main()
{
    int n,m;
    cin >> n >> m;
    vector<pair<int,int>> A(m,pair<int,int>());
    int a;
    rep(i,m){
        cin >> a;
        A.at(i) = make_pair(a,i);
    }
    vector<string> s(n);
    rep(i,n){
        cin >> s.at(i);
    }
    vector<int> score(n,0);
    rep(i,n){
        rep(j,m){
            if(s.at(i).at(j)=='o'){
                score.at(i) += A.at(j).first;
            }
        }
        score.at(i) += i+1;
    }
    vector<int> lack_score(n,0);
    int lack_max;
    rep(i,n){
        lack_max = 0;
        rep(j,n){
            lack_max = max(lack_max,score.at(j)-score.at(i));
        }
        if(lack_max!=0){
            lack_max++;
        }
        lack_score.at(i) = lack_max;
    }
    sort(ALL(A),greater<pair<int,int>>{});
    vector<vector<int>> B(n,vector<int>(1,0));
    rep(i,n){
        rep(j,m){
            if(s.at(i).at(A.at(j).second)=='o') continue;
            B.at(i).push_back(B.at(i).back() + A.at(j).first);
        }
    }
    rep(i,n){
        int j = 0;
        while(j<n+1&&lack_score.at(i)>B.at(i).at(j)){
            j++;
        }
        cout << j << endl;
    }
}