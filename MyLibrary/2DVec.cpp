#include <vector>
#include <iostream>

template <class T>
struct Vec2D
{
    Vec2D(int n) : height(1), width(n), vec(std::vector<T>(n, T())) {}
    Vec2D(int h, int w) : height(h), width(w), vec(std::vector<T>(h * w, T())) {}

    void fill(T x) { std::fill(vec.begin(), vec.end(), x); }

    int get_width() { return width; }
    int get_height() { return height; }
    int get_size() { return height * width; }

    T &operator[](int i) { return vec[i]; }
    const T &operator[](int i) const { return vec[i]; }

    T &operator[](int i, int j) { return vec[i * width + j]; }
    const T &operator[](int i, int j) const { return vec[i * width + j]; }

    std::pair<int, int> trans_index(int i) { return {i / width, i % width}; }
    int trans_index(int x, int y) { return x * width + y; }

    void print()
    {
        int i = 0;
        for (int &x : vec)
        {
            std::cout << x << " \n"[i % width == width - 1];
            ++i;
        }
    }

private:
    int width, height;
    std::vector<T> vec;
};

int main()
{
    Vec2D<int> vec(10, 10);
    vec.fill(3);
    vec.print();
    auto [i, j] = vec.trans_index(23);
    int idx = vec.trans_index(i, j);
    std::cout << idx << i << j << std::endl;
}