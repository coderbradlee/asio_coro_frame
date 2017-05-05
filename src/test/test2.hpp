/************************************************
 * libgo sample8
************************************************
 * libgo作为并发编程库使用。
************************************************/
#include <chrono>
#include <iostream>
#include <boost/thread.hpp>
#include "coroutine.h"
using namespace std;
using namespace std::chrono;

// 大计算量的函数
int c = 0;
void foo()
{
    int v = 1;
    for (int i = 1; i < 20000000; ++i)
        v *= i;
  c += v;
}
void test()
{
  co_chan<boost::unique_ptr<int>> ch_u;
  boost::unique_ptr<int> xx(new int(11));
  co_u<<boost::move(xx);
  boost::unique_ptr<int> out;
  co_u>>out;
  printf("%d\n", *out);
}
void test3()
{
  co_chan<boost::shared_ptr<int>> ch_1(1);
  go [=]{
    boost::shared_ptr<int> x(new int(2));
    ch_1<<x;
    (*x)++;
    ch_1<<x;
  };
  go [=]{
    boost::shared_ptr<int> x;
    ch_1>>x;
    printf("%d\n", *x);
    (*x)++;
    ch_1>>x;
    printf("%d\n", *x);
  };
  co_sched.RunUntilNoTask();
}
void test2()
{
  co_chan<int> ch_0;
  go [=]{
    ch_0<<1;
  };
  go [=]{
    int i;
    ch_0>>i;
    printf("%d\n", i);
  };  
  co_sched.RunUntilNoTask();

}
int test1()
{
    // 普通的for循环做法
    auto start = system_clock::now();
    for (int i = 0; i < 100; ++i)
        foo();
    auto end = system_clock::now();
    cout << "for-loop, cost ";
    cout << duration_cast<milliseconds>(end - start).count() << "ms" << endl;

    // 使用libgo做并行计算
    start = system_clock::now();
    for (int i = 0; i < 100; ++i)
        go foo;

    // 创建8个线程去并行执行所有协程 (由worksteal算法自动做负载均衡)
    boost::thread_group tg;
    for (int i = 0; i < 8; ++i)
        tg.create_thread([] {
                co_sched.RunUntilNoTask();
                });
    tg.join_all();

    end = system_clock::now();
    cout << "go with coroutine, cost ";
    cout << duration_cast<milliseconds>(end - start).count() << "ms" << endl;
    cout << "result zero:" << c * 0 << endl;
    return 0;
}

