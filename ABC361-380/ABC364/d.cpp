#include <bits/stdc++.h>
#define rep(i, n) for (int i = 0; i < (int)n; i++)
#define ALL(a) a.begin(),a.end()
using namespace std;
using ll = long long;

int main()
{
    int N, Q;
    cin >> N >> Q;
    vector<int> A(N);
    int point_max = -1 * 1e8;
    int point_min = 1e8;
    rep(i,N){
        cin >> A[i];
        point_max = max(point_max,A[i]);
        point_min = min(point_min,A[i]);
    }
    vector<int> B(Q), K(Q);
    rep(i,Q){
        cin >> B[i] >> K[i];
        point_max = max(point_max,B[i]);
        point_min = min(point_min,B[i]);
    }
    //k番目に近い点を求める
    //点Bjに対して距離がx以下の点はいくつあるか
    //これがk個になるようにする

    sort(ALL(A));

    rep(i,Q){
        int dist_r = point_max - point_min;
        int dist_l = 0;
        while(dist_r - dist_l > 1){
            int dist_mid = dist_l + (dist_r - dist_l) / 2;
        }
    }
}