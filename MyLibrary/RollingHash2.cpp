#include<bits/stdc++.h>
using namespace std;

// 必要ならばコンストラクタに数列の最大値を渡す
// 数列の値は1以上
struct RollingHash {
    using ull = unsigned long long;
    const ull MASK30 = (1UL << 30) - 1;
    const ull MASK31 = (1UL << 31) - 1;
    const ull MOD = (1UL << 61) - 1;
    const ull MASK61 = MOD;
    const ull POSITIVIZER = MOD*4;

    //a*b < mod*4
    ull Mull(ull a, ull b){
        ull au = a >> 31;
        ull ad = a & MASK31;
        ull bu = b >> 31;
        ull bd = b & MASK31;
        ull mid = ad*bu + au*bd;
        ull midu = mid >> 30;
        ull midd = mid & MASK30;
        return au*bu*2 + midu + (midd << 31) + ad*bd;
    }

    // mod 2^61-1
    ull CalcMod(ull x){
        ull xu = x >> 61;
        ull xd = x & MASK61;
        ull res = xu + xd;
        if(res >= MOD) res -= MOD;
        return res;
    }

    ull CalcGCD(ull a, ull b){
        if(a%b == 0UL) return b;
        return CalcGCD(b, a%b);
    }

    ull CalcPow(ull x, ull n){
        ull res = 1;
        while(n > 0){
            if(n & 1) res = CalcMod(Mull(res, x));
            n >>= 1;
            x = CalcMod(Mull(x, x));
        }
        return res;
    }

    // A : 数列Sの最大値
    ull RandBase(ull A = 127UL, ull r = 37UL){
        mt19937_64 mt(0);
        ull k;
        ull res;
        do{
            do{
                k = mt() % (MOD-1);
            }while(CalcGCD(k,MOD-1)!=1);
            res = CalcPow(r,k);
        }while(res <= A);
        return res;
    }

    ull base;
    vector<ull> base_pow;
    // 複数の文字列のハッシュを持つ
    vector<vector<ull>> hash;

    RollingHash(){
        base = RandBase();
        base_pow.emplace_back(1UL);
    }
    RollingHash(ull A){
        base = RandBase(A);
        base_pow.emplace_back(1UL);
    }

    void MakeHash(const string &S){
        ull i = hash.size();
        hash.emplace_back(vector<ull>());
        ull siz = S.size();
        if(base_pow.size() < siz){
            for(ull j=base_pow.size()-1; j<siz; ++j){
                base_pow.emplace_back(CalcMod(Mull(base_pow[j],base)));
            }
        }
        hash[i].emplace_back(0UL);
        for(ull j=0; j<siz; ++j){
            hash[i].emplace_back(CalcMod(Mull(hash[i][j],base) + (ull)S[j]));
        }
    }
    template<class T>
    void MakeHash(const vector<T> &S){
        ull i = hash.size();
        hash.emplace_back(vector<ull>());
        ull siz = S.size();
        if(base_pow.size() < siz){
            for(ull j=base_pow.size()-1; j<siz; ++j){
                base_pow.emplace_back(CalcMod(Mull(base_pow[j],base)));
            }
        }
        hash[i].emplace_back(0UL);
        for(ull j=0; j<siz; ++j){
            hash[i].emplace_back(CalcMod(Mull(hash[i][j],base) + (ull)S[j]));
        }
    }

    void AppendHash(int idx, const string &S){
        assert(hash.size() >= idx);
        if(hash.size() <= idx) hash.emplace_back(vector<ull>());
        if(hash[idx].size() == 0) hash[idx].emplace_back(0UL);
        ull siz = hash[idx].size()-1 + S.size();
        if(base_pow.size() < siz){
            for(ull j=base_pow.size()-1; j<siz; ++j){
                base_pow.emplace_back(CalcMod(Mull(base_pow[j],base)));
            }
        }
        ull offset = hash[idx].size()-1;
        for(ull j=offset; j<siz; ++j){
            hash[idx].emplace_back(CalcMod(Mull(hash[idx][j],base) + (ull)S[j-offset]));
        }
    }

    template<class T>
    void AppendHash(int idx, const vector<T> &S){
        assert(hash.size() >= idx);
        if(hash.size() <= idx) hash.emplace_back(vector<ull>());
        if(hash[idx].size() == 0) hash[idx].emplace_back(0UL);
        ull siz = hash[idx].size()-1 + S.size();
        if(base_pow.size() < siz){
            for(ull j=base_pow.size()-1; j<siz; ++j){
                base_pow.emplace_back(CalcMod(Mull(base_pow[j],base)));
            }
        }
        ull offset = hash[idx].size()-1;
        for(ull j=offset; j<siz; ++j){
            hash[idx].emplace_back(CalcMod(Mull(hash[idx][j],base) + (ull)S[j-offset]));
        }
    }

    // 0-indexed の半開区間[begin, end)
    ull GetHash(int idx, int begin_, int end_){
        return CalcMod(hash[idx][end_] + POSITIVIZER - CalcMod(Mull(hash[idx][begin_], base_pow[end_-begin_])));
    }
};

int main(){
    /* test */
    // string S = "ABC", T = "BBC";
    // RollingHash rh;
    // rh.MakeHash(S);
    // rh.MakeHash(T);
    // cout << rh.GetHash(0,1,3) << ' ' << rh.GetHash(1,1,3) << endl;
    // cout << rh.CalcPow(rh.base,1) << ' ' << rh.base_pow[1] << endl;
    // cout << (int)'a' << ' ' << (int)'A' << ' ' << (int)'0' << endl;
    // string U = "BBC";
    // rh.AppendHash(0,U);
    // cout << rh.GetHash(0,3,6) << ' ' << rh.GetHash(1,0,3) << endl;
    // rh.AppendHash(2,"hash");
    // cout << rh.GetHash(2,0,4) << endl;

    /* ABC 430 - E */
    // int T;
    // cin >> T;
    // while(T--){
    //     string A, B;
    //     cin >> A >> B;
    //     int N = A.size();
    //     A += A;
    //     int ans = -1;
    //     RollingHash rh;
    //     rh.MakeHash(A);
    //     rh.MakeHash(B);
    //     for(int i=0; i<N; ++i){
    //         if(rh.GetHash(0,i,N+i) == rh.GetHash(1,0,N)){
    //             ans = i;
    //             break;
    //         }
    //     }
    //     cout << ans << endl;
    // }
}