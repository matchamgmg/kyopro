using ull = unsigned long long;
const ull MASK30 = (1ULL << 30) - 1;
const ull MASK31 = (1ULL << 31) - 1;
const ull MOD = (1ULL << 61) - 1;
const ull MASK61 = MOD;
const ull POSITIVIZER = MOD*4;

/*a*b mod 2^61-1を返す関数(最後にmodをとる)
 *Rolling Hashに組み込む際は最後のmodを省略してよい
 *(オーバーフローしないことが保証される)
*/
ull Mull(ull a, ull b){
    ull au = a >> 31;
    ull ad = a & MASK31;
    ull bu = b >> 31;
    ull bd = b & MASK31;
    ull mid = ad * bu + au *bd;
    ull midu = mid >> 30;
    ull midd = mid & MASK30;
    return au*bu*2 + midu + (midd<<31) + ad*bd;
    //return CalcMod(au*bu*2 + midu + (midd<<31) + ad*bd);
}

/*mod 2^61-1を計算する関数*/
ull CalcMod(ull x){
    ull xu = x >> 61;
    ull xd = x & MASK61;
    ull res = xu + xd;
    if(res>=MOD) res -= MOD;
    return res;
}

ull GCD(ull a, ull b){
    if(a%b==0) return b;
    return GCD(b,a%b);
}

ull PowMod(ull base, ull exp){
    ull res = 1;
    while(exp>0){
        if(exp&1) res = CalcMod(Mull(res,base));
        exp >>= 1;
        base = CalcMod(Mull(base,base));
    }
    return res;
}

ull RandBase(ull base0, ull A, mt19937_64 &mt){
    ull res;
    ull k;
    do{
        k = mt()%(MOD-1);
        if(GCD(k,MOD-1)!=1) continue;
        res = PowMod(base0,k);
    }while(res<=A);
    return res;
}

struct RollingHash {

};