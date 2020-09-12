#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <random>

using namespace std;

const long N = 100000000;

template<class Iter>
void merge_sort(Iter first, Iter last)
{
    if (last - first > 1) {
        auto middle = first + (last - first) / 2;
        merge_sort(first, middle);
        merge_sort(middle, last);
        inplace_merge(first, middle, last);
    }
}
 
int main()
{   
    vector<int> v;
 
    for (int i = 0; i < N; ++i) 
        v.push_back(i);
        
    for (size_t i = 0; i < v.size(); ++i) 
        swap(v[i], v[rand() % (v.size() - i) + i]); 

    auto it = v.begin();

    merge_sort(v.begin(), v.end());
    for(auto n : v) {
        std::cout << n << ' ';
    }
    std::cout << '\n';
}