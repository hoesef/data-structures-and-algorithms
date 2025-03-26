#include <iostream>
#include <string>
#include <cstdint>

#include "..\include\queues\arr-queue.h"
#include "..\include\allocation-metrics\allocation-metrics.h"

class Foo {

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
            size = other.size;
            buff = new char[size];
            for (uint32_t i = 0; i < size; i++) {
                buff[i] = other.buff[i];
            }
            std::cout << "Foo Copy constructor called. Copying " << other.id << " into " << id << "\n";
        }
        Foo(Foo&& other) {
            id = uid++;
            size = other.size;
            buff = other.buff;
            other.buff = nullptr;
            other.size = 0;
            std::cout << "Foo Move constructor called. Moving " << other.id << " into " << id << "\n";
        }
        Foo& operator=(const Foo& other) {
            if (this != &other){
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

int main() {

    {
        arr_queue::Queue<Foo> q;

        Foo f1("Hello", 6);
        q.add(f1);
        std::cout << f1 << "\n";
        std::cout << q.pop() << "\n";
        printCurrentMemory();

        q.print();

    }

    printCurrentMemory();
    
    return 0;
}