#include <iostream>
#include <string>
#include <cstdint>

#include "..\include\allocation-metrics\allocation-metrics.h"
// #include "..\include\queues\arr-queue.h"
#include "..\include\stacks\arr-stack.h"

class Foo {

    public:
        static uint32_t moved;
        static uint32_t copied;
    
    private:
        static uint32_t uid;
        char* buff{nullptr};
        uint32_t size{0};
        uint32_t id{0};
    
    public:
        Foo(const char* cbuff, uint32_t nsize) {
            id = uid++;
            size = nsize;
            buff = new char[size];
            for (uint32_t i = 0; i < size; i++) {
                buff[i] = cbuff[i];
            }
            std::cout << "Foo Constructor " << id << " called \n";
        }
        Foo(const Foo& other) {
            id = uid++;
            copied++;
            size = other.size;
            buff = new char[size];
            for (uint32_t i = 0; i < size; i++) {
                buff[i] = other.buff[i];
            }
            std::cout << "Foo Copy constructor called. Copying " << other.id << " into " << id << "\n";
        }
        Foo(Foo&& other) {
            id = uid++;
            moved++;
            size = other.size;
            buff = other.buff;
            other.buff = nullptr;
            other.size = 0;
            std::cout << "Foo Move constructor called. Moving " << other.id << " into " << id << "\n";
        }
        Foo& operator=(const Foo& other) {
            if (this != &other){
                copied++;
                if (buff) {delete [] buff;}
                size = other.size;
                buff = new char[size];
                for (uint32_t i = 0; i < size; i++) {
                    buff[i] = other.buff[i];
                }
                std::cout << "Foo assignment operator called. Copying " << other.id << " into " << id << "\n";
            }
            return *this;
        }
        Foo& operator=(Foo&& other) {
            if (this != &other) {
                moved++;
                if (buff) { delete [] buff; }
                size = other.size;
                buff = other.buff;
                other.buff = nullptr;
                other.size = 0;
            }
            std::cout << "Foo move assignment operator called. Moving " << other.id << " into " << id << "\n";
            return *this;
        }
        uint32_t getNum() const { return id; }
        const char* getStr() const { 
            if (buff) {return buff; }
            else { return "\0"; }
        }
        ~Foo() {
            delete [] buff;
            std::cout << "Deleting " << id << "\n";
        }
};

std::ostream& operator<<(std::ostream& out, const Foo & f) {
    out << "Foo object " << f.getNum() << ": " << f.getStr();
    return out;
}

uint32_t Foo::uid = 1;
uint32_t Foo::moved = 0;
uint32_t Foo::copied = 0;

int main() {

    std::cout << "\n\n";
    
    /*
    {
        arr_queue::Queue<Foo> q;

        Foo f1("Hello", 6);
        q.add(f1);
        std::cout << f1 << "\n";
        // std::cout << q.pop() << "\n";
        printCurrentMemory();

        std::cout << "Q was: ";
        q.print();

        arr_queue::Queue<Foo> q2;
        q2 = std::move(q);
        std::cout << "Made q1, q is now: ";
        q.print();
        std::cout << "q2 is: ";
        q2.print();
    }

    printCurrentMemory();
    */

    {
        printCurrentMemory();

        arr_stack::Stack<Foo> s;

        Foo f1("Hello", 6);
        std::cout << s;
        s.add(f1);
        s.print();
        s.pop();
        s.print();
        for (int i = 0; i < 50; i++) {
            s.add(f1);
        }
        s.print();
        for (int i = 0; i < 50; i++) {
            s.pop();
            s.print();
        }
        s.print();

        printCurrentMemory();
    }

    printCurrentMemory();

    std::cout << "Total copied objects: " << Foo::copied << "\n";
    std::cout << "Total moved objects: " << Foo::moved << "\n";

    return 0;
}