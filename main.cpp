//
//  main.cpp
//  CProject
//
//  Created by Sagar Negi on 4/4/19.
//  Copyright © 2019 Sagar Negi. All rights reserved.
//

#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
using namespace std;
atomic<int> counter(0);

auto current_time()
{
    return chrono::system_clock::now();
}
auto elapsed_time(chrono::time_point<chrono::system_clock> &start, chrono::time_point<chrono::system_clock> &end)
{
    return chrono::duration_cast<std::chrono::milliseconds>(end - start);
}
void increase_counter()
{
    int j = 0;
    while (j < 250000000) {
        int old_value = counter.load();
        while (!counter.compare_exchange_weak(old_value, old_value + 1, memory_order_release, memory_order_relaxed))
        {
            _mm_pause();
        }
        j++;
    }
    
}


int main() {
    
    auto start = current_time();
    thread t1(increase_counter);
    thread t2(increase_counter);
    t1.join();
    t2.join();
    cout<<counter;
    auto end = current_time();
    auto elapsed = elapsed_time(start, end);
    cout<<elapsed.count();
}
/*#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include <mutex>
using namespace std;
mutex mtx;
auto current_time()
{
    return chrono::system_clock::now();
}
auto elapsed_time(chrono::time_point<chrono::system_clock> &start, chrono::time_point<chrono::system_clock> &end)
{
    return chrono::duration_cast<std::chrono::milliseconds>(end - start);
}
void single_thread()
{
    int counter = 0;
    for(size_t i=0;i<500000000;i++)
        counter++;
    cout<<counter<<endl;
}
void single_thread_lock()
{
    int counter = 0;
    for(size_t i=0;i<500000000;i++)
    {
        mtx.lock();
        counter++;
        mtx.unlock();
    }
    cout<<counter<<endl;
}
void single_thread_atomic()
{
    atomic<int> counter(0);
    for(size_t i=0;i<500000000;i++)
        counter++;
    cout<<counter<<endl;
}
void single_thread_volatile()
{
    volatile int counter = 0;
    for(size_t i=0;i<500000000;i++)
        counter++;
    cout<<counter<<endl;
}
void mutli_thread(int &counter)
{
    for(size_t i=0;i<250000000;i++)
    {
        mtx.lock();
        counter++;
        mtx.unlock();
    }
}
void multi_thread_atomic(atomic<int> &counter)
{
    for(size_t i=0;i<250000000;i++)
        counter++;
}
int main(int argc, const char * argv[]) {
    // insert code here...
    auto start = current_time();
    single_thread();
    auto end = current_time();
    auto elapsed = elapsed_time(start, end);
    cout<<"Single Thread "<<elapsed.count()<<endl;
    
    start = current_time();
    single_thread_atomic();
    end = current_time();
    elapsed = elapsed_time(start, end);
    cout<<"Single Thread Atomic "<<elapsed.count()<<endl;
    
    start = current_time();
    single_thread_lock();
    end = current_time();
    elapsed = elapsed_time(start, end);
    cout<<"Single Thread lock "<<elapsed.count()<<endl;
    
    start = current_time();
    single_thread_volatile();
    end = current_time();
    elapsed = elapsed_time(start, end);
    cout<<"Single Thread volatile "<<elapsed.count()<<endl;
    
    int counter = 0;
    start = current_time();
    thread t1(mutli_thread, ref(counter));
    thread t2(mutli_thread, ref(counter));
    t1.join();
    t2.join();
    cout<<counter<<endl;
    end = current_time();
    elapsed = elapsed_time(start, end);
    cout<<"Two Thread lock "<<elapsed.count()<<endl;
    
    
    atomic<int> atmcounter(0);
    start = current_time();
    thread t3(multi_thread_atomic, ref(atmcounter));
    thread t4(multi_thread_atomic, ref(atmcounter));
    t3.join();
    t4.join();
    cout<<atmcounter<<endl;
    end = current_time();
    elapsed = elapsed_time(start, end);
    cout<<"Two Thread atomic "<<elapsed.count()<<endl;
    return 0;
}*/
