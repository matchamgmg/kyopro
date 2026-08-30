#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
using vi = vector<int>;
using vll = vector<ll>;
using vld = vector<ld>;
using vmint = vector<mint>;
using vvi = vector<vi>;
using vvll = vector<vll>;
using vvld = vector<vld>;
using vvmint = vector<vmint>;
using vvvi = vector<vvi>;
using vvvll = vector<vvll>;
using vvvld = vector<vvld>;
using vvvmint = vector<vvmint>;
template<class T> using pq = priority_queue<T, vector<T>>;//大きい順
template<class T> using pq_g = priority_queue<T, vector<T>, greater<T>>;//小さい順

#define rep(i, s, n) for (int i = s; i < (n); i++)
#define rrep(i, s, n) for (int i = (n) - 1; i >= s; i--)
#define all(a) a.begin(),a.end()
#define rall(a) a.rbegin(),a.rend()
#define chmax(x,y) x = max(x,y)
#define chmin(x,y) x = min(x,y)
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
#define next_p(a) next_permutation(all(a))
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n)rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}

int main()
{
    int N;
    cin >> N;
    vi A(N);
    rep(i,0,N){
        cin >> A[i];
    }
    A.push_back(N+1);
    // start_pos : 1連続で登場する数の後ろ 3連続で登場する数の後ろから2番目
    //  数kが登場したとき，その数が最後に登場した位置の後ろ
    // end_pos : 数kが2連続以上続くとき　+= 2
    // ans_len : end_pos - start_pos; 計算タイミング : end_pos, start_posの更新直後
    // 数kが2連続で続くとき end_pos += 2
    // 数kが3連続以上続くとき はじめ2つと終わり2つだけ考える
    int num = 0;
    int cmb = 0;
    vi last_pos(N+1,-1);
    int start_pos = 0;
    int end_pos = 0;
    int max_len = 0;
    rep(i,0,N+1){
        if(A[i]!=num){
            if(cmb>=2){
                end_pos += 2;
                if(last_pos[num]>=start_pos){
                    start_pos = last_pos[num] + 1;
                }
                chmax(max_len,end_pos-start_pos);
                if(cmb>2){
                    start_pos = i - 2;
                    end_pos = i;
                }
                chmax(max_len,end_pos-start_pos);
            }else{
                start_pos = i;
                end_pos = i;
                chmax(max_len,end_pos-start_pos);
            }
            last_pos[num] = i-1;
            num = A[i];
            cmb = 1;
        }else{
            cmb++;
        }
    }
    cout << max_len << endl;
}