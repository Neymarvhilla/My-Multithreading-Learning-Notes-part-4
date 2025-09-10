#include <iostream>
#include <vector>
#include <thread>
#include <future>
#include <chrono>
#include <algorithm>
using namespace std::literals;

/*
 * - Futures and Promises
 * */

/*
 * Transferring Data Between Threads
 *
 *
 * - std::thread does not provide a way to return a value
 *      - So far, we have used a shared variable
 *      - Access to the shared variable needs to be protected by locks
 *
 * - Condition variables allow us to coordinate threads
 *      - A thread can signal to another thread that shared data has been modified
 *      - Cannot directly transfer data from one thread to another
 *      */


/*
 * std::future and std::promise
 *
 * - Classes for transferring data between threads
 * - Together, these set up "shared state" between threads
 * - The shared state can transfer data from one thread to another
 *      - No shared data variables
 *      - No explicit locking
 *      */

/*
 * Producer-Consumer Model
 * - Futures and promises use a producer - consumer model
 *      - Reader/writer threads are an example of this model
 * - A "producer" thread will generate a result
 * - A "consumer" thread waits for the result
 * - The producer thread generates the result
 * - The producer thread stores the result in the shared state
 * - The consumer thread reads the result from the shared state
 * */

/*
 * Transfer of Data using Future and Promise
 *
 * - An std::promise is associated with the producer
 * - An std::future object is associated with the consumer
 *      - The consumer calls a member function of the future object
 *      - The function blocks until the result becomes available
 *
 * - The producer thread sends the result
 *      - The promise object stores the result in the shared state
 *
 * - The consumer thread receives the result
 *      - The member function reads the result from the shared state
 *      - The member function returns the result
 *      */

/*
 * Exception Handling
 * - Futures and promises also work with exceptions
 *      - The promise stores the exception in the shared state
 * - This exception will be rethrown in the consumer thread
 *      - By the future's blocking function
 * - The producer thread "throws" the exception to the consumer
 * */


/*
 * - A promise object is associated with a future object
 * - Together, they create a "shared state"
 *      - The promise object stores a result in the shared state
 *      - the future object gets the result from the shared state
 *      */

/*
 * std::future
 *
 * - Represents a result that is not yet available
 * - One of the most important classes in C++ concurrency
 *      - Works with many different asynchronous objects and operations
 *      - Not just std::promise
 * - An std::future object is not usually created directly
 *      - Obtained from an std::promise object
 *      - Or returned by asynchronous operation
 *      */

/*
 * std::future
 *
 * - Template class defined in <future>
 *      - The parameter is the type of the data that will be returned
 *
 * - get() member function
 *      - Obtains the result when ready
 *      - Blocks until the operation is complete
 *      - Fetches the result and returns it
 *
 * - wait() and friends
 *      - Block but do not return a result
 *      - wait() blocks until the operation is complete
 *      - wait_for() and wait_until() block with a timeout
 *      */

/*
 * std::promise
 *
 * - Template class defined in <future>
 *      - The parameter is the type of the result
 *
 * - Constructor
 *      - Creates an associated std::future object
 *      - Sets up the shared state with it
 *
 * - get_future() member function
 *      - Returns the associated future
 *          std::promise<int> prom;
 *          std::future fut = prom.get_future();
 *          */

/*
 * std::promise interface
 *
 * - get_future() member function
 *      - Returns the std::future object associated with this promise
 *
 * - set_value()
 *      - Sets the result to its argument
 *
 * - set_exception()
 *      - Indicates that an exception has occurred
 *      - This can be stored in the shared state
 *      */

/*
 * Producer-Consumer Model
 *
 * - Parent thread
 *      - Creates an std::promise object
 *
 * - Producer task function
 *      - Takes the std::promise object as argument
 *      - Calls set_value()
 *          - Or set_exception()
 * - Consumer task function
 *      - Takes the associated std::future object as argument
 *      - Call get()
 *          - Or wait() and friends
 *          */

/*
 * - Parent thread
 *      - Creates an std::promise object
 *
 * - Producer task function
 *      - Takes the std::promise object as argument
 *      - Calls set_value()
 *          - or set_exception
 *
 * - Consumer task function
 *      - Takes the associated std::future object as argument
 *      - Calls get()
 *          - Or wait() and friends
 *          */


/*
 *
 * // the producer's task function an std::promise as argument
 *
 *
 *          void produce(std::promise<int> &px)
 *          {
 *              // produce the result
 *              int x = 42;
 *
 *              // Store the result in the shared state
 *              px.set_value(x);
 *          }
 *          */

/*
 *
 *
 * // the consumer's task function takes an std::future as argument
 *
 *          void consume(std::future<int> &fx)
 *          {
 *              // Get the result from the shared state
 *              int x = fx.get();
 *          }
 *          */

/*
 * Parent Thread Example
 *
 *      // create an std::promise object
 *      std::promise<int> prom;
 *
 *
 *      // get the associated future
 *      std::future<int> fut = prom.get_future();
 *
 *      // The producer task function takes the promise as argument
 *      std::thread thr_produce(produce, std::ref(prom));
 *
 *      // the consumer task function takes the future as argument
 *      std::thread thr_consumer(consumer, std::ref(fut));
 *      */

/*
 * Producer-Consumer with Exception Handling
 *
 * - In the producer thread
 *      - Put a try block around code that might throw
 *      - In the catch block, call set_exception() on the promise
 *      - This captures the active exception
 *
 * - set_exception() takes a pointer to the exception object
 *      - We can use a catch-all handler
 *      - Pass the return value from std::current_exception()
 *      */

/*
 * Producer-Consumer with Exception Handling
 *
 * - In the consumer thread
 *      - Put a try block around the call to get() or wait()
 *      - Write a catch block to handle the exception
 *      */

/*   Producer with Exception Handling
 *
 *          void produce(std::promise<int> &px)
 *          {
 *              try{
 *
 *                  // Code that may throw
 *                  ....
 *
 *                  // If no exception, store the result in the shared state
 *                  px.set_value(x);
 *              }
 *              catch (...) {
 *                  // Exception caught - store it in the shared state
 *                  px.set_exception(std::current_exception());
 *              }
 *          }*/

/*
 * Consumer with Exception Handling
 *
 *
 *          void consume (std::future <int> &fx)
 *          {
 *              try {
 *                  // Get the result from the shared state - may throw
 *                  int x = fx.get();
 *              }
 *              catch (...) {
 *                  // Exception thrown - get it from the shared state
 *              }
 *          }
 *          */

/*
 * Producer with std::make_exception_ptr()
 *
 *  - To throw an exception ourselves, we could
 *      - Throw the exception inside a try block
 *      - Write a catch block that calls set_exception()
 *
 *  - C++ has std::make_exception_ptr()
 *      - Takes the exception object we want to throw
 *      - Returns a pointer to its argument
 *      - Pass this pointer to set_exception()
 *      - Avoid "boilerplate" code
 *      - Better code generation
 *      */

/*
 * Producer with std::make_exception_ptr()
 *
 *
 *      void produce(std::promise<int>& ptx)
 *      {
 *
 *          if(...) {
 *              px.set_exception(std::make_exception_ptr(std::out_of_range("Oops")));
 *              return;
 *          }
 *
 *          // store the result in the shared state
 *          px.set_value(x)
 *      }
 *      */

/*
 * Single Producer with Multiple Consumer
 *
 * - Single producer thread
 *      - Produces a result or an event
 * - Multiple consumer threads
 *      - Use the result
 *      - Or wait for the event to occur
 *
 * - Used in many applications
 * */

/*
 * std::future and Multiple Waiting Threads
 * - Designed for use with single consumer thread
 *      - Assumes it has exclusive read access to the shared state
 * - Cannot be safely shared between threads
 *      - Data race
 * - Cannot be copied
 *      - Move-only class
 *      */

/*
 * std::shared_future
 * - Can be copied
 *      - Each thread has its own object
 *      - They all share the same state with the std::promise
 *      - Calling get() or wait() from different copies is safe
 *      */

/*
 * Obtaining an std::shared_future object
 *
 * - Normally, we do not create a shared future directly
 * - We can move from an existing std::future
 *      - std::shared_future<int> shared_fut1 = std::move(fut);
 * - We can also call share() on the std::future
 *      - std::shared_future<int> shared_fut2 = fut.share();
 * - We can also obtain a shared_future directly from a promise
 *      - shared_future<int> shared_fut3 = prom.get_future();
 *      */

/*
 * std::shared_future Example
 * - The producer will be the same as before
 * - The consumer now takes an std::shared_future
 *          void consume(std::shared_future<int> &fx);*/

/*
 *      // Parent thread
 *      std::promise<int> prom;
 *
 *      // Move the promise's future into a shared future
 *
 *      std::shared_future<int> shared_fut1 = prom.get_future();
 *
 *      // Copy the shared future object
 *      std::shared_future<int> shared_fut2 = shared_fut1;
 *
 *      // Start two consumer threads, each with its own shared future object
 *      std::thread thr_consumer1(consume, std::ref(shared_fut1));
 *      std::thread thr_consumer2(consume, std::ref(shared_fut2));
 *      std::thread thr_producer(produce, std::ref(prom));
 *      */

/*
 *
 *
 *          class File_handle {
 *
 *              FILE* p;
 *          public:
 *              File_handle(const char* pp, const char* r) {
 *                  p = fopen(pp, r); if (p==0) throw File_error(pp, r);
 *              }
 *              File_handle(const string& s, const char *r) {
 *                  p = fopen(s.c_str(), r); if(p==0) throw File_error(s,r);
 *              }
 *              ~Filehandle() {
 *                  fclose(p);
 *              }
 *          };
 *          */

int &multiply_2(int &x) {
    x = x * 2;
    return x;
}

void produce(std::promise<int> &px)
{
    try {
        // produce the result
        int x = 42;
        std::this_thread::sleep_for(2s);

        // code that may throw
        if (0) {
            throw std::out_of_range("Oops");
        }

        // No exception - store the result in the shared state
        // store the result in the shared state
        std::cout << "Promise sets shared state to: " << x << std::endl;

        px.set_value(x);
    }

    catch (...) {
        // Exception thrown - store it in the shared state
        px.set_exception(std::current_exception());
    }

}


void consume(std::future<int> &fx)
{
    // Get the result from the shared state
    std::cout << "Future calling get()......" << std::endl;
    try {
        int x = fx.get();
        std::cout << "Future returns from calling get()" << std::endl;
        std::cout << "The answer is " << x << std::endl;
    }
    catch(std::exception &e) {
        // Exception thrown - get it from the shared state
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
}

// Dangling Pointers
int *global = nullptr;
void g(int *p) {
    global = p;  // not Ok: trying to store away the pointer
}

void f(int *p)  {
    int x = 4;
    g(&x); // Ok
    g(p); // Ok
}

// How to avoid and catch dangling pointers

void g(std::vector<int *> &v)
{
    //....
}

std::vector<int *> f1(int *p)
{
    int x = 4;
    std::vector<int *> res = {p, &x, new int{7}};
    g(res);
    return res;
}
int main() {
    std::cout << "Hello, World!" << std::endl;

    // Create an std::promise object
    // This creates an associated std::future object
    // and sets up a shared state between them


    int a = 6;
    int b = a;
    int *ptr1 = &a;
    int *ptr2 = ptr1;

    std::promise<int> prom;

    // Get the future associated with the promise
    std::future<int>fut = prom.get_future();

    // start the threads
    // the producer task function takes the promise as argument

    std::thread thr_producer(produce, std::ref(prom));

    // The consumer task function takes the future as argument
    std::thread thr_consumer(consume, std::ref(fut));

    thr_producer.join(), thr_consumer.join();


    std::vector<int> my_vec = {1,2,3,4,45};
    std::for_each(my_vec.begin(), my_vec.end(), [] (int &x) {x = x * 2;});
    for (auto &value : my_vec) {
        std::cout << value << "\n";
    }




    return 0;
}
