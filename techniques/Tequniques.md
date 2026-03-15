# ABC449-C 頻度配列を差分更新して$O(\sigma +N)$で解く

$i\leq j, ~ L\leq j-i \leq R \leftrightarrow j-R \leq i \leq j-L$

$j$を固定すると$i$の範囲は上式で表される

```C++
int N, L, R;
cin >> N >> L >> R;
++R;
string S;
cin >> S;
ll ans = 0;
vector<int> cnt(26, 0);

for(int j=0; j<N; ++j){
    if(j >= L) cnt[S[j-L] - 'a']++;
    if(j >= R) cnt[S[j-R] - 'a']--;
    ans += cnt[S[j] - 'a'];
}

cout << ans << endl;
```

# ABC449-E 要素の追加＋K番目取得
* FenwickTreeを用いる
* SegmentTree::min_left
* $\text{data}[i] \in \{0,1\}$
* OrderedSetにおいてK-th elementの取得ができるような実装もある

順序付き（多重）集合（SortedList）における$K$番目の要素は$\sum_{i=0}^{x}\text{data}[i]\geq K$を満たす最小の$x$