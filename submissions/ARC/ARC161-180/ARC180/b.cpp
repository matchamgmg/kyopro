#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;
using tup = tuple<int,int,int>;
using prique = priority_queue<tup>;

vector<vector<bool>> check;

int search(vector<int> &P,int &l,int &r,int &k,int &now){
    vector<int> pp = P;
    int n = pp.size();
    int cntp1 = 0,cntp2 = 0;
    rep(i,n){
        if(abs(l-i)<k) continue;
        if(i<l&&pp[i]<pp[l]) continue;
        if(l<i&&pp[l]<pp[i]) continue;
        if(check[i][l]==true||check[l][i]==true) continue;
        cntp1++;
    }
    rep(i,n){
        if(i<r&&pp[i]<pp[r]) continue;
        if(l<r&&pp[r]<pp[i]) continue;
        if(check[i][r]==true||check[r][i]==true) continue;
        if(i)
        cntp2++;
    }
    swap(pp[l],pp[r]);
    int cntpp1 = 0,cntpp2 = 0;
    rep(i,n){
        if(abs(l-i)<k) continue;
        if(i<l&&pp[i]<pp[l]) continue;
        if(l<i&&pp[l]<pp[i]) continue;
        if(check[i][l]==true||check[l][i]==true) continue;
        if(i==r) continue;
        cntpp1++;
    }
    rep(i,n){
        if(abs(r-i)<k) continue;
        if(i<r&&pp[i]<pp[r]) continue;
        if(l<r&&pp[r]<pp[i]) continue;
        if(check[i][r]==true||check[r][i]==true) continue;
        if(i==l) continue;
        cntpp2++;
    }
    int sub = cntp1 + cntp2 - cntpp1 - cntpp2;
    return now - sub;
}

int main()
{
    int N,K;
    cin >> N >> K;
    vector<int> P(N);
    rep(i,N){
        cin >> P[i];
        check.push_back(vector<bool>(N,false));
    }
    vector<prique> ans;
    int now = 0;
    for(int i=0;i<N-1;i++){
        for(int j=i+1;j<N;j++){
            if(j-i<K) continue;
            if(P[i]<P[j]) continue;
            now++;
        }
    }
    prique pq0;
    pq0.push({now,-1,-1});
    ans.push_back(pq0);
    int m = 0;
    while(true){
        m++;
        prique pq;
        for(int i=0;i<N-1;i++){
            for(int j=i+1;j<N;j++){
                if(j-i<K) continue;
                if(P[i]<P[j]) continue;
                if(check[i][j]==true) continue;
                int a = get<0>(ans.back().top());
                pq.push({search(P,i,j,K,a),i,j});
                check[i][j] = true;
            }
        }
        ans.push_back(pq);
        int d = get<0>(ans.back().top());
        if(d==0) break;
    }
    cout << m << endl;
    for(int i=1;i<ans.size();i++){
        int a = get<1>(ans[i].top());
        int b = get<2>(ans[i].top());
        cout << (a+1) << " " << (b+1) << endl;
    }
}