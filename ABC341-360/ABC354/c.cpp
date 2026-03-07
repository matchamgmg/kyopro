#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)

int main()
{
    int N;
    cin >> N;
    vector<tuple<int,int,int>> aci(N,tuple<int,int,int>());
    int a,c;
    rep(i,N){
        cin >> a >> c;
        aci.at(i) = make_tuple(a,c,i);
    }
    sort(aci.begin(),aci.end(),greater<tuple<int,int,int>>{});
    vector<int> index;
    int last_clear;
    rep(i,N){
        if(i==0){
            index.push_back(get<2>(aci.at(i)));
            last_clear=i;
        }else{
            if(get<1>(aci.at(i))<=get<1>(aci.at(last_clear))){
                index.push_back(get<2>(aci.at(i)));
                last_clear=i;
            }
        }
    }
    sort(index.begin(),index.end());
    cout << index.size() << endl;
    for(auto id:index){
        cout << id+1 << " ";
    }
    cout << endl;
}