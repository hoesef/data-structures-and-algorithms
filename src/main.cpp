#include <iostream>
#include <string>
#include <cstdint>

#include "..\include\allocation-metrics\allocation-metrics.h"
#include "..\include\smart-pointers\unique-pointer.h"
#include "..\include\smart-pointers\shared-pointer.h"

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
    
    printCurrentMemory();

    // {
    //     smrt_ptr::Unique_pointer<Foo> ptr1 = smrt_ptr::make_unique<Foo>("Hello", 6);
    //     smrt_ptr::Unique_pointer<Foo> ptr2 = smrt_ptr::make_unique<Foo>("world", 6);

    //     std::cout << *ptr1 << " " << *ptr2 << "\n";

    //     ptr1.swap(ptr2);

    //     std::cout << *ptr1 << " " << *ptr2 << "\n";

    //     std::cout << "ptr1: " << ptr1->getStr() << "\n";
    //     std::cout << "ptr2: " << ptr2->getStr() << "\n";

    //     ptr1.~Unique_pointer();

    // }

    {
        smrt_ptr::Shared_pointer<Foo> shrd_ptr = smrt_ptr::make_shared<Foo>("Hello", 6);
        std::cout << *shrd_ptr << "\n";
        {
            smrt_ptr::Shared_pointer<Foo> shrd_ptr2(shrd_ptr);
            std::cout << *shrd_ptr2 << "\n";
            shrd_ptr.reset(new Foo("world", 6));
            shrd_ptr.swap(shrd_ptr2);

            std::cout << shrd_ptr->getStr() << " " << shrd_ptr2->getStr() << "\n";

            std::cout << "Foo obj 1 ref count: " << shrd_ptr.use_count() << "\n";
            std::cout << "shrd_ptr is unique: " << shrd_ptr.unique() << "\n";

            smrt_ptr::Shared_pointer<Foo> shrd_ptr3;
            shrd_ptr3 = shrd_ptr;
            smrt_ptr::Shared_pointer<Foo> shrd_ptr4(shrd_ptr3);
            
            std::cout << "Foo obj 1 ref count: " << shrd_ptr.use_count() << "\n";
            std::cout << "shrd_ptr is unique: " << shrd_ptr.unique() << "\n";

            printCurrentMemory();

        }

        std::cout << "Foo obj 1 ref count: " << shrd_ptr.use_count() << "\n";
        std::cout << "shrd_ptr is unique: " << shrd_ptr.unique() << "\n";


        std::cout << *(shrd_ptr.get()) << "\n";
    }

    printCurrentMemory();

    return 0;
}