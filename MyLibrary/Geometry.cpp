#include<bits/stdc++.h>
using namespace std;

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
        return = abs(Outer(PS, PG) / VecSize(SG));
    }

    template<class T>
    long double NormalLineLength(vector<T> &PS, vector<T> &PG){
        vector<T> SG = DiffVec(PS, PG);
        return = abs(Outer(PS, PG) / VecSize(SG));
    }

    // 点と線分の最短距離
    template<class T>
    long double Point2LineDistance(vector<T> &P, vector<T> &S, vector<T> &G){
        vector<T> PS = DiffVec(S, P), PG = DiffVec(G, P), SG = DiffVec(G, S);
        if(Inner(PS, SG) > -eps) return VecSize(PS);
        if(Inner(PG, SG) < eps) return VecSize(PG);
        return abs(Outer(PS, PG) / VecSize(SG));
    }

    template<class T>
    long double Point2LineDistance(vector<T> &PS, vector<T> &PG){
        vector<T> SG = DiffVec(PG, PS);
        if(Inner(PS, SG) > -eps) return VecSize(PS);
        if(Inner(PG, SG) < eps) return VecSize(PG);
        return abs(Outer(PS, PG) / VecSize(SG));
    }
}

int main(){
    vector<int> O = {0,0}, A = {1,1}, B = {3,1};
    cout << Geo2Vec::Point2LineDistance(O,A,B) << ' ' << Geo2Vec::NormalLineLength(O,A,B) << endl;
}