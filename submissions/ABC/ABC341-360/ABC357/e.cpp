#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;



int main()
{
    int N;
    cin >> N;
    vector<int> A(N);
    rep(i,N){
        cin >> A[i];
        A[i]--;
    }
    vector<bool> used(N,false);//頂点からの探索を終えたか
    vector<int> idx(N,-1);
    vector<int> ord;
    vector<int> cnt(N);//頂点vから到達可能な頂点の数

    //サイクル計算
    rep(sv,N) if(!used[sv]){//全ての頂点を順番に見る
        int v = sv;//sv...始点
        vector<int> vs;//vを始点とするパス
        while(1){
            if(used[v]) break;//すでに探索済みの頂点
            if(idx[v] != -1){//サイクル検出
                vector<int> cyc(vs.begin()+idx[v],vs.end());//vector<int>(iterator,iterator)
                for(int u : cyc) cnt[u] = cyc.size();//サイクルのサイズ
                break;
            }
            idx[v] = vs.size();//vからsvまでの長さ
            vs.push_back(v);
            v = A[v];
        }
        for(int v : vs) used[v] = true;//パス上の頂点をすべて探索済みにする
        reverse(ALL(vs));
        ord.insert(ord.end(),ALL(vs));//パスの逆順を並べる
    }

    for(int v : ord){//サイクル内の頂点から順にみていく
        if(cnt[v] != 0) continue;//サイクル上の頂点は除く..すでに調べたサイクル外の頂点ものぞく
        cnt[v] = cnt[A[v]] + 1;//サイクルから出た
    }

    ll ans = 0;
    rep(i,N) ans += cnt[i];
    cout << ans << endl;
}