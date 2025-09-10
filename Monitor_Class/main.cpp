#include <iostream>
#include <mutex>
#include <thread>

using namespace std::literals;




/*
 * Monitor Class
 *
 * - Internally Synchronized
 *      - Java synchronized object
 *      */

/*
 * Bank Class
 *
 * // Vert simple "bank" class
 *
 *
 *          class Bank:
 *          public:
 *              void debit(const std::string &name, int amount);
 *              void credit(const std::string &name, int amount);
 *              void print(const std::string &name);
 *          };
 *
 *          */

/*
 * Naive Solution
 * - Mutex as private member
 * - Evert member function which accesses shared data locks the mutex
 *
 *              class Bank {
 *              // Mutex to protect the data
 *              std::mutex mut;
 *
 *              // Shared data
 *              ...
 *
 *              public:
 *                  void debit(const std::string &name, int amount) {
 *                      std::lock_guard(mut);
 *                      ....
 *                  }
 *
 *                  void credit(const std::string &name, int amount) {
 *                      std::lock_guard(mut);
 *                      ...
 *                  }
 *              };
 *              */

/*
 * Naive Solution Drawbacks
 *
 * - Member functions may need to call other member functions
 *      - Using multiple locks risks deadlock
 *
 * - Transactions may involve multiple member functions calls
 * - Results in many locking and unlocking operations
 *      - Race conditions
 *      - Potential data race
 * - Existing classes need to be modified
 * */


/*
 * Monitor Class
 *
 * - We write a wrapper class
 *      - The Bank object is a data member
 *      - The member functions lock a mutex and forward to the Bank object
 *
 *      class BankMonitor {
 *          std::mutex mut;
 *          Bank bank;
 *
 *      public:
 *          void debit(const std::string &name, int amount) {
 *              // Lock the mutex and forward the call
 *              std::lock_guard lck(mut);
 *              bank.debit(name, amount);
 *          }
 *
 *
 *      */


/*
 * Monitor Class
 *
 * - Works with any type
 *      - Including classes that were not designed for threaded code
 *      - No modifications needed to the class
 *
 * - Does not help callers who want to perform transactions
 *      - Unnecessary locking
 *      - Possibility of deadlock due to multiple locking
 *      - Allows interruptions by other threads
 *      */

// Very simple "Bank" class

class Bank {
    // Mutex to protect the data
    //std::mutex mut;

public:
    void debit(const std::string &name, int amount)
    {
        //std::lock_guard<std::mutex> lck(mut);
        std::cout << "Debiting " << amount << " from " << name << std::endl;
    }

    void credit(const std::string &name, int amount)
    {
        //std::lock_guard<std::mutex> lck(mut);
        std::cout << "Crediting " << amount << " from " << name << std::endl;
    }

    void print( const std::string &name)
    {
        std::cout << name << "'s transaction went through" << std::endl;
    }
};


class BankMonitor {
    std::mutex mut;
    Bank bank;

public:
    void debit(const std::string &name, int amount)
    {
        // Lock the mutex and forward the call
        std::lock_guard<std::mutex> lck(mut);
        bank.debit(name, amount);
    }

    void credit(const std::string &name, int amount)
    {
        std::lock_guard<std::mutex> lck(mut);
        bank.debit(name, amount);
    }

    void print (const std::string &name)
    {
        std::lock_guard<std::mutex> lck(mut);
        bank.print(name);
    }


};

// Monitor class
// It wraps the protected object
// Each member function locks the mutex when called
// Then forwards the call to the wrapped object



int main() {
    std::cout << "Hello, World!" << std::endl;


    // Transfer $1000 from Peter to Paul
    Bank bank;

    // Call its member functions
    // Each member function locks separately

    std::thread thr([&bank] {bank.debit("Peter",1000);
    bank.credit("Paul", 1000);
    bank.print("Peter");
    bank.print("Paul");});

    thr.join();
    std::cout << "Done" << std::endl;

    BankMonitor bank_mon;

    // Call its member functions
    // Each member function locks separately

    std::thread thr1([&bank_mon] {
        bank_mon.debit("Peter", 1000);
        bank_mon.credit("Paul", 1000);
        bank_mon.print("Peter");
        bank_mon.print("Paul");
    });
    thr1.join();
    std::cout << "Done" << std::endl;
    return 0;
}
