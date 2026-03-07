# Mo's Algorithm
* 区間の端をひとつ伸ばす・ひとつ縮める操作に対する結果を高速に処理できるとき、長さ$N$の静的な列に対する$Q$個のオフライン区間クエリを$O(N\sqrt{Q})$回の伸ばす・縮める操作で処理する
* 区間クエリの結果がマージできないときに使う
* クエリを適切に並べ替えるパート
* 並べ替えた順にクエリを伸ばす・縮める操作で処理するパート
* クエリの並べ替え・・・マンハッタン距離上の巡回セールスマン問題に対して良い解を与えることだと考えることができる

## クエリの並べ替え
* クエリの$l,r$を二次元平面上にプロットしたものについて，$Q$個の頂点間の移動回数を考える
* $l$軸方向を$\sqrt{Q}$個に分割する
* 分割された領域内で$r$の昇順に頂点を移動する最短経路について考える
* $r$軸方向の移動回数 : $N$
* $l$軸方向の移動回数 : クエリ一回当たり高々$N/\sqrt{Q}$
* すべての領域についての移動距離を合計すると移動距離は$O(2N\sqrt{Q} + Q*N/\sqrt{Q})=O(N\sqrt{Q})$
```C++
int N; // 数列の長さ
int Q; // クエリの数
vector<int> L(Q), R(Q); // クエリの[l,r)
/* 入力 */

// クエリのソート
int SQ = max(1, int(N/sqrt(Q))); // 領域の分割数
vector<int> qidx(Q);
for(int i=0; i<Q; ++i) qidx[i] = i;
sort(qidx.begin(), qidx.end(), [&](int i, int j){ // クエリの順番をpair(l/sq, r)でソート
    if(L[i]/SQ != L[j]/SQ) return L[i]<L[j];
    return R[i]<R[j];
});

// クエリの処理
// add(i) : 追加操作
// del(i) : 削除操作
int nl = 0, nr = 0;
for(int i : qidx){
    while(nl>L[i]) --nl, add(nl);
    while(nr<R[i]) add(nr), ++nr;
    while(nl<L[i]) del(nl), ++nl;
    while(nr>R[i]) --nr, del(nr);
    /* クエリの計算結果 */
}
```
## 補足
* ヒルベルト曲線に沿ってクエリをソートすると定数倍がよくなるらしい
```C++
/* Hilbert Order Sort */
int N;
int maxN = 1;
while(maxN<N){
    maxN<<=1;
}
int Q;
vector<int> L(Q), R(Q);
vector<int> qidx(Q);
for(int i=0; i<Q; ++i) qidx[i] = i;

vector<long long> Hilbert_val(Q);
auto Hilbert_order = [&](int x, int y) -> long long {
    long long rx, ry, d;
    d = 0;
    for(long long s=maxN>>1; s>0; s>>=1){
        rx = (x&s)>0, ry = (y&s)>0;
        d += s*s*((rx * 3) ^ ry);
        if(ry>0) continue;
        if(rx>0){
            x = maxN-1 - x;
            y = maxN-1 - y;
        }
        swap(x, y);
    }
    return d;
};

for(int i=0; i<Q; ++i) Hilbert_val[i] = Hilbert_order(L[i], R[i]);
sort(qidx.begin(), qidx.end(), [&](int i, int j){
    return Hilbert_val[i]<Hilbert_val[j];
});

```
### 木上のMo
* DFSの行きがけ順に頂点を並べ替えることで様々なクエリに対してMoを適用できる
#### 部分木に対するクエリ
* 各頂点を配列の区間に対応させることができる
#### パスに対するクエリ
* オイラーツアーの訪問順に頂点を並べ替える