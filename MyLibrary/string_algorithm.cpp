#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

/* Morris-Pratt algorithm
 * 文字列の各文字を終端とする部分文字列 [0,i) について
 * prefixとsuffixの最大一致文字数を求める
*/
vector<int> MP_algorithm(string &S){
    int N = S.size();
    vector<int> border(N+1,-1);
    int j = -1;
    for(int i=0;i<N;i++){
        while(j>=0 && S[i]!=S[j]){// ここがtrueになる回数は全体で高々N回
            j = border[j];
        }
        j++;
        border[i+1] = j;
    }
    return border;
}

/* Knuth Morris-Patt algorithm
 * MPの各ステップの計算量を改善したもの
 * S[border[j]]==S[j]であるとき，明らかにS[i]!=S[j]であるから
 * この無駄な遷移を省略する
 * 各ステップの最悪計算量 : O(N) -> O(logN)
 * 全体の計算量 : O(N) -> O(N)
 * こっちは文字列全体の最長borderを計算する(上位互換ではない)
 * 各ステップごとのjの値はMPに一致
 */
vector<int> KMP_algorithm(string &S){
    int N = S.size();
    vector<int> border(N+1,-1);
    int j = -1;
    // cout << j << ' ';
    for(int i=0;i<N;i++){
        while(j>=0 && S[i]!=S[j]){// ここがtrueになる回数は全体で高々O(logN)回
            j = border[j];
        }
        j++;
        if(S[i+1]==S[j]) border[i+1] = border[j];
        else border[i+1] = j;
        // cout << j << ' ';
    }
    // cout << endl;
    return border;
}

/* Manacher algorithm
 * 文字列中の各文字を中心とする回文の最大半径を求める : O(N)
 * 奇数長の回文しか検出できないが、各文字の間にダミー文字を挟むことで偶数長の回文を検出できる
*/

vector<int> Manacher_algorithm(string &S, bool odd, char dummy){
    string S2;
    int N;
    if(odd){
        N = S.size();
        S2 = S;
    }else{
        N = S.size()*2-1;
        for(int i=0;i<N;i++){
            if(i%2==0){
                S2.push_back(S[i/2]);
            }else{
                S2.push_back(dummy);
            }
        }
    }
    // manacher
    vector<int> R(N,0);
    int i = 0, j = 0;
    while(i<N){
        while(i-j>=0 && i+j<N && S2[i-j]==S2[i+j]) j++;
        R[i] = j;
        int k = 1;
        while(i-k>=0 && k+R[i-k]<j){
            R[i+k] = R[i-k];
            k++;
        }
        i += k;
        j -= k;
    }
    return R;
}

/* Z algorithm
 * 文字列とその部分列 [i,N) について
 * prefixの最長共通文字数を構築する : O(N)
*/

vector<int> Z_algorithm(string &S){
    int N = S.size();
    vector<int> prefix(N,0);
    prefix[0] = N;
    // i: 窓の先頭
    // j: prefix一致数
    int i = 1, j = 0;
    while(i<N){
        // prefixの一致数をカウントアップして記録する（0または途中から）
        while(i+j<N && S[j]==S[i+j]) j++;
        prefix[i] = j;
        if(j==0){
            i++;
            continue;
        }
        // prefix arrayの値を利用する
        int k = 1;
        while(i+k<N && k+prefix[k]<j){
            prefix[i+k] = prefix[k];
            k++;
        }
        // カウントアップの初期値を再設定（jの値）
        i += k;
        j -= k;
    }
    return prefix;
}

int main(){
    string s = "abacabacababaca";
    // vector<int> a = MP_algorithm(s);
    // for(int &x : a) cout << x << ' ';
    // cout << endl;
    // vector<int> b = KMP_algorithm(s);
    // for(int &x : b) cout << x << ' ';
    // cout << endl;
    // vector<int> c = Manacher_algorithm(s,true,'.');
    // for(int &x : c) cout << x << ' ';
    // cout << endl;
    // vector<int> d = Manacher_algorithm(s,false,'.');
    // for(int i=0;i<s.size()-1;i++){// 偶数長の回文の半径を取り出すとき
    //     cout << d[i*2+1]/2 << ' ';
    // }
    // cout << endl;
    // for(int i=0;i<s.size();i++){// 奇数長の回文の半径を取り出すとき
    //     cout << (d[i*2]+1)/2 << ' ';
    // }
    // cout << endl;
    vector<int> e = Z_algorithm(s);
    for(int &x : e) cout << x << ' ';
    cout << endl;
}