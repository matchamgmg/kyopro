#include<bits/stdc++.h>
using namespace std;

struct Testcase {
    int N, K, T;
    mt19937 mt;
    vector<string> wv, wh;
    vector<pair<int,int>> target;
    
    Testcase(){}
    Testcase(int n, int k, int t, int seed) : N(n), K(k), T(t) {
        mt = mt19937(seed);
        wv.resize(n);
        wh.resize(n-1);
        target.resize(k);
        for(string &s : wv){
            s.assign(n-1,'0');
        }
        for(string &s : wh){
            s.assign(n,'0');
        }
        vector<int> X(n*n);
        int i, j;
        iota(X.begin(),X.end(),0);
        for(int x=0; x<100000; x++){
            i = mt()%(n*n);
            do{
                j = mt()%(n*n);
            }while(i==j);
            swap(X[i],X[j]);
        }
        for(int x=0; x<k; x++){
            target[x].first = X[x]/n;
            target[x].second = X[x]%n;
        }
    }

    void fprint(){
        ofstream ofs("testcase.txt");
        ofs << N << ' ' << K << ' ' << T << '\n';
        for(string &s : wv){
            ofs << s << '\n';
        }
        for(string &s : wh){
            ofs << s << '\n';
        }
        for(auto &p : target){
            ofs << p.first << ' ' << p.second << '\n';
        }
    }
};

int main(){
    Testcase tc = Testcase(6,36,800,0);
    tc.fprint();
}