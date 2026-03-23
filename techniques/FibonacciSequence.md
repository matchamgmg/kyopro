# Fibonacci Sequence (フィボナッチ数列)
### 定義
```math
\begin{align*}
(& F_0 = 0) \\
& F_1 = 1 \\
& F_2 = 1 \\
& F_n = F_{n-1} + F_{n-2} \qquad (n \geq 3)
\end{align*}
```
### 一般項
```math
F_n = \frac{1}{\sqrt{5}}\Biggl\{\biggl( \frac{1+\sqrt{5}}{2}\biggr) ^{n} - \biggl( \frac{1-\sqrt{5}}{2}\biggr) ^{n}\Biggr\}
= \frac{\varphi^n - (1-\varphi)^n}{\sqrt{5}}
= \frac{\varphi^n - (-\varphi)^{-n}}{\sqrt{5}}
```
### 一般項の近似式
$\frac{1}{\sqrt{5}} = 0.447\dots$より，$F_n$の値を$0.447$以下の誤差で近似できる
```math
F_n \fallingdotseq \frac{\varphi^n}{\sqrt{5}}
```
### 行列表現
```math
\begin{pmatrix}
F_{n+1} & F_{n} \\
F_{n}   & F_{n-1}
\end{pmatrix}
=
\begin{pmatrix}
1 & 1 \\
1 & 0
\end{pmatrix}
^{n}
\times
\begin{pmatrix}
F_1 & F_0 \\
F_0 & F_{-1}
\end{pmatrix}
```
$F_1 = F_2 = 1$のとき
```math
\begin{pmatrix}
F_{n+1} & F_{n} \\
F_{n}   & F_{n-1}
\end{pmatrix}
=
\begin{pmatrix}
1 & 1 \\
1 & 0
\end{pmatrix}
^{n}
\times
\begin{pmatrix}
1 & 0 \\
0 & 1
\end{pmatrix}
=
\begin{pmatrix}
1 & 1 \\
1 & 0
\end{pmatrix}
^{n}
```
$F_1 = x, ~ F_2 = y$のとき，$F_0 = y-x$より
```math
\begin{pmatrix}
F_{n+1} & F_{n} \\
F_{n}   & F_{n-1}
\end{pmatrix}
=
\begin{pmatrix}
1 & 1 \\
1 & 0
\end{pmatrix}
^{n-1}
\times
\begin{pmatrix}
y & x \\
x & y-x
\end{pmatrix}
```
```math
\begin{pmatrix}
F_{n} & F_{n-1} \\
F_{n-1}   & F_{n-2}
\end{pmatrix}
=
\begin{pmatrix}
1 & 1 \\
1 & 0
\end{pmatrix}
^{n-2}
\times
\begin{pmatrix}
y & x \\
x & y-x
\end{pmatrix}
```

### 競プロにおける計算量の見積もり
フィボナッチサーチなど
```math
\begin{align*}
& \log_{\varphi}{x} = \log_{2}{x} ~ / ~ \log_{2}{\varphi} \fallingdotseq 1.44 \times \log_{2}{x} \\
& O(\log{x})
\end{align*}
```

### 性質
フィボナッチ数列には様々な良い性質がある（累和や累積についての性質が多い）