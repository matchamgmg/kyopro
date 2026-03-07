#include<bits/stdc++.h>
using namespace std;

template<class T>
struct Compression1D {
    vector<int> comp;
    vector<T> val;
    Compression1D(){}
    Compression1D(const vector<T> &v) : comp(v.size()), val(v) {
        sort(val.begin(),val.end());
        val.erase(unique(val.begin(), val.end()), val.end());
        for(int i=0; i<v.size(); ++i){
            comp[i] = lower_bound(val.begin(), val.end(), v[i]) - val.begin();
        }
    }
    friend ostream& operator << (ostream &s, const Compression1D &c) {
        for(auto x : c.comp){
            s << x << ' ';
        }
        return s;
    }
};


// 圧縮率をもっと高める方法があるかも
template<class T>
struct Compression2D {
    vector<int> compX1, compX2, compY1, compY2;
    vector<T> valX, valY;
    Compression2D(){}
    Compression2D(const vector<T> &X1, const vector<T> &Y1, const vector<T> &X2, const vector<T> &Y2, bool space = false)
    : compX1(X1.size()), compX2(X2.size()), compY1(Y1.size()), compY2(Y2.size()) {
        if(space){
            for(int i=0; i<X1.size(); ++i){
                for(T d=0; d<2; ++d){
                    valX.emplace_back(X1[i]+d);
                    valX.emplace_back(X2[i]+d);
                    valY.emplace_back(Y1[i]+d);
                    valY.emplace_back(Y2[i]+d);
                }
            }
        }else{
            for(int i=0; i<X1.size(); ++i){
                valX.emplace_back(X1[i]);
                valX.emplace_back(X2[i]);
                valY.emplace_back(Y1[i]);
                valY.emplace_back(Y2[i]);
            }
        }
        sort(valX.begin(), valX.end());
        sort(valY.begin(), valY.end());
        valX.erase(unique(valX.begin(), valX.end()), valX.end());
        valY.erase(unique(valY.begin(), valY.end()), valY.end());
        for(int i=0; i<X1.size(); ++i){
            compX1[i] = lower_bound(valX.begin(), valX.end(), X1[i]) - valX.begin();
            compX2[i] = lower_bound(valX.begin(), valX.end(), X2[i]) - valX.begin();
            compY1[i] = lower_bound(valY.begin(), valY.end(), Y1[i]) - valY.begin();
            compY2[i] = lower_bound(valY.begin(), valY.end(), Y2[i]) - valY.begin();
        }
    }
    friend ostream& operator << (ostream &s, const Compression2D &c) {
        for(auto x : c.compX1){
            s << x << ' ';
        }
        s << endl;
        for(auto x : c.compY1){
            s << x << ' ';
        }
        s << endl;
        for(auto x : c.compX2){
            s << x << ' ';
        }
        s << endl;
        for(auto x : c.compY2){
            s << x << ' ';
        }
        return s;
    }
};

int main(){
    // unique {1,2,2,2,2,3,4} -> {1,2,3,4,2,2,2}
    vector<int> a = {1,9,6,6,13,20};
    Compression1D<int> ca(a);
    cout << ca << endl;

    // 右，下について，空白部分を考慮するため，連結成分が混ざらない
    vector<int> X1 = {1,5,4,0}, Y1 = {0,1,3,7}, X2 = {1,5,9,8}, Y2 = {3,8,3,7};
    Compression2D<int> c2(X1, Y1, X2, Y2, true);
    cout << c2 << endl;

    Compression2D<int> c3(X1, Y1, X2, Y2);
    cout << c3 << endl;
}