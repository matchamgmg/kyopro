#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
using ld = long double;
using mint = modint998244353;
//using mint = modint1000000007;
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
using vb = vector<bool>;
using vvb = vector<vb>;
template<class T> using pq = priority_queue<T, vector<T>>;//大きい順
template<class T> using pq_g = priority_queue<T, vector<T>, greater<T>>;//小さい順

#define rep(i, s, n) for (int i = s; i < (int)n; i++)
#define rrep(i, s, n) for (int i = (int)n - 1; i >= s; i--)
#define all(a) a.begin(),a.end()
#define rall(a) a.rbegin(),a.rend()
#define chmax(x,y) x = max(x,y)
#define chmin(x,y) x = min(x,y)
#define pyes cout << "Yes" << endl
#define pno cout << "No" << endl
#define YN(a) if(a){ pyes; }else{ pno; }
#define v_cout(a) {int n = a.size(); rep(i,0,n) cout << a[i] << " "; cout << endl;}
#define vv_cout(a) {int n = a.size(); rep(i,0,n){rep(j,0,a[i].size())cout << a[i][j] << " "; cout << endl;}cout << endl;}
#define grid_check(x,y,X,Y) (0<=x && x<X && 0<=y && y<Y)
#define input(A) rep(i,0,A.size()) cin >> A[i]

namespace Geo2Vec {
    const long double eps = 1e-10;

    template<class T>
    struct Line {
        vector<T> start, vec;
        vector<long double> t_limit;

        Line() {}
        Line(vector<T> &start_, vector<T> &vec_) : start(start_), vec(vec_) {
            t_limit.resize(2);
            t_limit[0] = -(1e10);
            t_limit[1] = 1e10;
        }
        Line(vector<T> &start_, vector<T> &vec_, long double t_min, long double t_max) : start(start_), vec(vec_) {
            t_limit.resize(2);
            t_limit[0] = t_min;
            t_limit[1] = t_max;
        }

        vector<long double> GetPoint(long double t){
            t = max(t, t_limit[0]);
            t = min(t, t_limit[1]);
            vector<long double> res(2);
            res[0] = (long double)start[0] + t*vec[0];
            res[1] = (long double)start[1] + t*vec[1];
            return res;
        }
    };

    template<class T>
    vector<T> MakeVec(T x1, T y1, T x2, T y2){
        vector<T> res(2);
        res[0] = x2 - x1;
        res[1] = y2 - y1;
        return res;
    }

    template<class T>
    long double VecSize(vector<T> &A){
        return sqrt((long double)(A[0]*A[0] + A[1]*A[1]));
    }

    template<class T>
    vector<long double> NormVec(vector<T> &A){
        long double siz = VecSize(A);
        vector<long double> res(2);
        res[0] = A[0]/siz;
        res[1] = A[1]/siz;
        return res;
    }

    template<class T>
    vector<T> AddVec(vector<T> &A, vector<T> &B){
        vector<T> res(2);
        res[0] = A[0] + B[0];
        res[1] = A[1] + B[1];
        return res;
    }

    template<class T>
    vector<T> DiffVec(vector<T> &A, vector<T> &B){
        vector<T> res(2);
        res[0] = A[0] - B[0];
        res[1] = A[1] - B[1];
        return res;
    }

    template<class T>
    T Outer(vector<T> &A, vector<T> &B){
        return A[0]*B[1] - A[1]*B[0];
    }

    template<class T>
    T Inner(vector<T> &A, vector<T> &B){
        return A[0]*B[0] + A[1]*B[1];
    }

    // 垂線の長さ
    template<class T>
    long double NormalLineLength(vector<T> &P, vector<T> &S, vector<T> &G){
        vector<T> PS = DiffVec(S, P), PG = DiffVec(G, P), SG = DiffVec(G, S);
        long double length = abs(Outer(PS, PG) / VecSize(SG));
        return length;
    }

    template<class T>
    long double NormalLineLength(vector<T> &PS, vector<T> &PG){
        vector<T> SG = DiffVec(PS, PG);
        long double length = abs(Outer(PS, PG) / VecSize(SG));
        return length;
    }

    // 点と線分の最短距離
    template<class T>
    long double Point2LineDistance(vector<T> &P, vector<T> &S, vector<T> &G){
        long double length;
        vector<T> PS = DiffVec(S, P), PG = DiffVec(G, P), SG = DiffVec(G, S);
        if(Inner(PS, SG) * Inner(PG, SG) <= 0){
            length = abs(Outer(PS, PG) / VecSize(SG));
        }else{
            length = min(VecSize(PS), VecSize(PG));
        }
        return length;
    }

    template<class T>
    long double Point2LineDistance(vector<T> &PS, vector<T> &PG){
        // long double length;
        vector<T> SG = DiffVec(PG, PS);
        if(Inner(PS, SG) > -eps) return VecSize(PS);
        if(Inner(PG, SG) < eps) return VecSize(PG);
        return abs(Outer(PS, PG) / VecSize(SG));
        // if(Inner(PS, SG) * Inner(PG, SG) <= 0){
        //     assert(Inner(PS, SG) <= 0);
        //     assert(Inner(PG, SG) >= 0);
        //     length = abs(Outer(PS, PG) / VecSize(SG));
        //     if(abs(Inner(PS, SG)) < eps){
        //         assert(abs(VecSize(PS) - length) < 1e-6);
        //     }
        //     if(abs(Inner(PG, SG)) < eps){
        //         assert(abs(VecSize(PG) - length) < 1e-6);
        //     }
        // }else{
        //     if(Inner(PS, SG) >= 0) length = VecSize(PS);
        //     if(Inner(PG, SG) <= 0) length = VecSize(PG);
        //     // length = min(VecSize(PS), VecSize(PG));
        // }
        // return length;
    }
}

void solve(){
    vector<vector<long double>> SG(2,vector<long double>(4));
    rep(i,0,2){
        rep(j,0,4){
            cin >> SG[i][j];
        }
    }
    long double distT, distA;
    vector<long double> O(2,0);
    vector<long double> OA = Geo2Vec::MakeVec(0.L, 0.L, SG[0][0], SG[0][1]);
    vector<long double> OB = Geo2Vec::MakeVec(0.L, 0.L, SG[0][2], SG[0][3]);
    vector<long double> OC = Geo2Vec::MakeVec(0.L, 0.L, SG[1][0], SG[1][1]);
    vector<long double> OD = Geo2Vec::MakeVec(0.L, 0.L, SG[1][2], SG[1][3]);

    vector<long double> AB = Geo2Vec::DiffVec(OB, OA), CD = Geo2Vec::DiffVec(OD, OC);
    // distT = Geo2Vec::VecSize(AB);
    // distA = Geo2Vec::VecSize(CD);
    auto calc_dist = [&](vector<long double> &a) -> long double {
        long double res = (a[2]-a[0])*(a[2]-a[0]) + (a[3]-a[1])*(a[3]-a[1]);
        return sqrt(res);
    };
    distT = calc_dist(SG[0]);
    distA = calc_dist(SG[1]);

    if(distA > distT){
        swap(distA, distT);
        swap(AB, CD);
        swap(OA, OC);
        swap(OB, OD);
    }
    // Aoki が止まるまで : Aoki を原点とした相対位置
    vector<long double> CA = Geo2Vec::DiffVec(OA, OC);
    vector<long double> NormAB = Geo2Vec::NormVec(AB), NormCD = Geo2Vec::NormVec(CD);
    vector<long double> V = Geo2Vec::DiffVec(NormAB, NormCD);
    Geo2Vec::Line<long double> lineAT(CA, V, 0, distA);
    vector<long double> CF = lineAT.GetPoint(distA);
    long double ans = 100000;
    chmin(ans, Geo2Vec::Point2LineDistance(CA,CF));
    // Aoki が止まった後
    if(distT > distA){
        Geo2Vec::Line lineAB(OA,NormAB,0,distA);
        vector<long double> OE = lineAB.GetPoint(distA);
        vector<long double> DE = Geo2Vec::DiffVec(OE, OD);
        vector<long double> DB = Geo2Vec::DiffVec(OB, OD);
        chmin(ans, Geo2Vec::Point2LineDistance(DE,DB));
    }
    printf("%.12Lf\n",ans);
}

int main()
{
    int T;
    cin >> T;
    while(T--){
        solve();
    }
}