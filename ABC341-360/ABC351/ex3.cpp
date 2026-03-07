#include <bits/stdc++.h>
using namespace std;

int main()
{
    int N;
    cin >> N;
    // ball's size
    vector<unsigned int> A(N);
    vector<unsigned int> ball;
    int new_ball, j = 0;

    // j...last index of ball
    for (int i = 0; i < N; i++, j++)
    {
        cin >> A.at(i);
        ball.push_back(A.at(i));

        while (true)
        {
            // section1
            if (ball.size() <= 1)
            {
                break;
            }

            // section2
            else if (ball.at(j) != ball.at(j - 1))
            {
                break;
            }

            // section3
            else
            {
                new_ball = ball.at(j);
                ball.pop_back();
                ball.pop_back();
                ball.push_back(new_ball+1);
                j--;
            }
        }
    }
    cout << j << endl;
}