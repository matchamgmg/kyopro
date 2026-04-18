#include <bits/stdc++.h>
using namespace std;

template <class T>
struct RunLength
{
    vector<pair<T, int>> code;
    RunLength() {}
    RunLength(const vector<T> &v)
    {
        pair<T, int> word = make_pair(v[0], 1);
        for (int i = 1; i < v.size(); ++i)
        {
            if (v[i] == word.first)
            {
                word.second++;
            }
            else
            {
                code.emplace_back(word);
                word = make_pair(v[i], 1);
            }
        }
        if (word.second > 0)
        {
            code.emplace_back(word);
        }
    }
    RunLength(const string &v)
    {
        pair<T, int> word = make_pair(v[0], 1);
        for (int i = 1; i < v.size(); ++i)
        {

            if (v[i] == word.first)
            {
                word.second++;
            }
            else
            {
                code.emplace_back(word);
                word = make_pair(v[i], 1);
            }
        }
        if (word.second > 0)
        {
            code.emplace_back(word);
        }
    }

    vector<pair<T, int>> &get_code()
    {
        return code;
    }

    friend ostream &operator<<(ostream &s, RunLength &rl)
    {
        for (pair<T, int> p : rl.code)
        {
            s << "(" << p.first << ", " << p.second << ") ";
        }
        return s;
    }
};

int main()
{
    string s = "aabbc";
    RunLength<char> rl(s);
    cout << rl << endl;

    vector a = rl.get_code();
    for (auto p : a)
    {
        cout << "(" << p.first << ", " << p.second << ") ";
    }
    cout << endl;
    a[0].first = 'd';
    cout << rl << endl;
    for (auto p : a)
    {
        cout << "(" << p.first << ", " << p.second << ") ";
    }
}