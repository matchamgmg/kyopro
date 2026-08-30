#include <bits/stdc++.h>
using namespace std;
int main(){
    int N,K;
    cin >> N>>K;
    vector<int> A;
    int a;
    for(int i=0;i<N;i++){
        cin>>a;
        if(a%K==0){
            A.push_back(a/K);
        }
    }
    for(int i=0;i<A.size();i++){
        cout<<A.at(i);
        cout <<(i!=A.size()-1?" ":"\n");
    }
}