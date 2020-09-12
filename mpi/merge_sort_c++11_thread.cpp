#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <random>
#include <functional>
#include <thread>

using namespace std;

const long N = 100000000;


void merge_sort(vector<int>::iterator first, vector<int>::iterator last)
{
    if (last - first > 1)
    {
        auto middle = first + (last - first) / 2;
        merge_sort(first, middle);
        merge_sort(middle, last);
        inplace_merge(first, middle, last);
    }
}


int main()
{   
    int NumThreads = 1;
    vector<int> v;
    vector<thread> threads;
 
    for (int i = 0; i < N; ++i) 
        v.push_back(i); 
    for (size_t i = 0; i < v.size(); ++i) 
        swap(v[i], v[rand() % (v.size() - i) + i]); 

    auto it = v.begin();
    int step = N / NumThreads;

    for (int i = 0; i < NumThreads; i++)
    {
        threads.push_back(
            thread(merge_sort, it + i * step, it + (i + 1) * step));    
    }

    for (int i = 0; i < NumThreads; i++)
    {
        threads[i].join();
    }

    for (int i = 1; i < NumThreads; i++)
    {
        inplace_merge(it + (i - 1) * step, it + i * step, it + (i + 1) * step);
    };
    /*for(auto n : v) {
        std::cout << n << ' ';
    }
    std::cout << '\n';*/
};