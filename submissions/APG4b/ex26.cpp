#include <bits/stdc++.h>
using namespace std;

void set_int(vector<string> &command, map<string, int> &ints)
{
    int x;
    for (int i = 3; command.at(i) != ";"; i++)
    {
        if (i == 3)
        {
            if (isdigit(command.at(i).at(0)))
            {
                x = stoi(command.at(i));
            }
            else
            {
                x = ints[command.at(i)];
            }
            continue;
        }
        if (i % 2 == 1 && command.at(i - 1) == "+")
        {
            if (isdigit(command.at(i).at(0)))
            {
                x += stoi(command.at(i));
            }
            else
            {
                x += ints[command.at(i)];
            }
        }
        else if (i % 2 == 1 && command.at(i - 1) == "-")
        {
            if (isdigit(command.at(i).at(0)))
            {
                x -= stoi(command.at(i));
            }
            else
            {
                x -= ints[command.at(i)];
            }
        }
    }
    ints[command.at(1)] = x;
    cout << command.at(1) << " = " << ints[command.at(1)] << endl;
}

void print_int()
{
}

void set_vec(vector<string> &command, vector<int> &ints, map<string, vector<int>> &vecs)
{
    vector<int> x, xx;
    int vec_start;
    int vec_size;
    bool first_factor = true;
    bool isvec = false;
    for (int i = 0; command.at(i) != ";"; i++)
    {
        if (command.at(i) == "[")
        {
            isvec = true;
            vec_start = i;
        }
        if (isvec)
        {
            if(isdigit(command.at(i).at(0))){
                xx.pus
            }
        }
        else if (command.at(i) != "+" && command.at(i) != "-")
        {//out of [], ref other vec
            if (first_factor)
            {
                x = vecs.at(command.at(i));
                first_factor=false;
            }
            else
            {
                xx = vecs.at(command.at(i));
                if (command.at(i - 1) == "+")
                {
                    for (int j = 0; j < x.size(); j++)
                    {
                        x.at(j) += xx.at(j);
                    }
                }
                else if (command.at(i - 1) == "-")
                {
                    for (int j = 0; j < x.size(); j++)
                    {
                        x.at(j) -= xx.at(j);
                    }
                }
            }
        }
        if (command.at(i) == "]")
        {
            isvec = false;
            if(first_factor){
                first_factor=false;
            }
        }
    }
}

void print_vec()
{
}

void read_command(vector<string> &command, map<string, int> &ints, map<string, vector<int>> &vecs)
{
    if (command.at(0) == "int")
    {
        set_int(command, ints);
    }
    else if (command.at(0) == "print_int")
    {
    }
    else if (command.at(0) == "vec")
    {
    }
    else if (command.at(0) == "print_vec")
    {
    }
}

int main()
{
    int N;
    cin >> N;
    vector<vector<string>> command(N);
    string str;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0;; j++)
        {
            cin >> str;
            command.at(i).push_back(str);
            if (command.at(i).at(j) == ";")
                break;
        }
    }

    map<string, int> ints;
    map<string, vector<int>> vecs;

    for (int i = 0; i < N; i++)
    {
        read_command(command.at(i), ints, vecs);
    }
}