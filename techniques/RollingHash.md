## RollingHash

文字列$S$について
$$
\mathrm{hash}(S) = \sum_{i=0}^{|S|-1} {S_i\cdot \mathrm{base}^{|S|-i-1}}\\
\mathrm{hash}(S[a, b)) = \mathrm{hash}(S[0, b)) - \mathrm{hash}(S[0, a))\cdot \mathrm{base}^{b-a}
$$
* $\mathrm{base}^i, \mathrm{hash}(S[0,i))$を前計算する
### 工夫
* 数字への割り当てが0となるような文字があると，ハッシュが衝突する
```
a -> 0
baaa = b
```
* 基数（base）を実行時にランダムに決定する
* mod = $2^{61}-1$
* $a,b < \mathrm{mod}$であるとき，$a*b < \mathrm{mod}*4$．そのため$a*b + \mathrm{int32\_t}$はオーバーフローしない
* 負の方向へのオーバーフローに注意
* $2^{61}-1$の原始根を使う

原始根 : $1\leq r<p$ なる$r$に関して
$r^{k} (1\leq k\leq p-2)\mathrm{mod}\,p\neq 0$
* 原始根は素数$p$に対して複数あることもある
* 素数$p$の原始根の一つがわかっているとき，$p-1$と互いに素な自然数$k$をランダムに生成して，$\mathrm{base} = r^k$とすればよい
* $p-1$と互いに素な自然数を乱数で見つける期待値はほぼ定数時間
* 数列の最大値を$A$として，$\mathrm{base} \leq A$のとき，衝突する数列がある
`base = 2, [1,2], [4]`
* $2^{61}-1$の原始根の一つは37