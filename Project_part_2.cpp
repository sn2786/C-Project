#include <iostream>
#include <list>
#include <random>
#include <chrono>
#include <thread>
#include <mutex>
#include <atomic>
#include <algorithm>
using namespace std;
mutex mtx;
struct use
{
    int a;
    chrono::time_point<chrono::system_clock> t;
};
list<use> l1;
list<use> l2;
list<use> l3;
double avg = 0;
double mini = INT_MAX;
double maxi = INT_MIN;
auto current_time()
{
    return chrono::system_clock::now();
}
auto elapsed_time(chrono::time_point<chrono::system_clock> &start, chrono::time_point<chrono::system_clock> end)
{
    return chrono::duration_cast<std::chrono::microseconds>(end - start);
}
void producer1()
{
    for(int i = 0;i<500000;i++)
    {
        use temp;
        temp.a = i;
        mtx.lock();
        temp.t = chrono::system_clock::now();
        l1.push_back(temp);
        mtx.unlock();
        this_thread::sleep_for (std::chrono::microseconds(1));
        cout<<"a";
    }
    avg/=500000;
    cout<<avg<<" "<<mini<<" "<<maxi;
}
void consume1()
{
    while(1)
    {
        mtx.lock();
        if(l1.size())
        {
            auto temp = l1.front();
            l1.pop_front();
            l2.push_back(temp);
            cout<<"b";
        }
        mtx.unlock();
    }
}
void consume2()
{
    while(1)
    {
        mtx.lock();
        if(l2.size())
        {
            auto temp = l2.front();
            l2.pop_front();
            l3.push_back(temp);
            cout<<"c";
        }
        mtx.unlock();
    }
}
void consume3()
{
    while(1)
    {
        mtx.lock();
        if(l3.size())
        {
            auto temp = l3.front();
            l3.pop_back();
            double time_taken = elapsed_time(temp.t, chrono::system_clock::now()).count();
            avg+= time_taken;
            mini = min(mini,time_taken);
            maxi = max(maxi,time_taken);
            cout<<"d";
        }
        mtx.unlock();
    }
}
void single_consumer_single_producer()
{
    
    thread t1(producer1);
    thread t2(consume1);
    thread t3(consume2);
    thread t4(consume3);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}
int main() {
    single_consumer_single_producer();
}
