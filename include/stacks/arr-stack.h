#ifndef ARRAY_STACK_H_
#define ARRAY_STACK_H_

#include <iostream>
#include <cstdint>
#include <stdexcept>

namespace arr_stack {

template <typename T> class Stack {

    private:
        T* m_ptr{nullptr};
        uint32_t m_size{1};
        uint32_t m_end{0};

    public:
        Stack() {
            std::cout << "arr_stack::Stack default constructor called\n";
            m_ptr = static_cast<T*>(operator new(sizeof(T) * m_size));          // alocate raw memory
        }
        Stack(const Stack<T>& other) {
            std::cout << "arr_stack::Stack copy constructor called\n";
            m_size = other.m_size;
            m_end = other.m_end;

            m_ptr = static_cast<T*>(operator new(sizeof(T) * m_size));          // allocate new memory

            for (uint32_t i = 0; i < m_end; i++) {
                new (&m_ptr[i]) T(other.m_ptr[i]);                              // copy data into memory
            }
        }
        Stack(Stack&& other) {
            std::cout << "arr_stack::Stack move constructor called\n";
            m_size = other.m_size;
            m_end = other.m_end;
            m_ptr = other.m_ptr;                                                // assume ownership of memory

            other.m_ptr = nullptr;                                              // strip ownership of memory from other
            other.m_size = 0;
            other.m_end = 0;
        }
        Stack& operator=(const Stack& other) {
            std::cout << "arr_stack::Stack copy assignment operator called\n";
            if (this != &other) {
                if (m_ptr) {
                    for (uint32_t i = 0; i < m_size; i++) {
                        m_ptr[i].~T();
                    }
                    operator delete(m_ptr);
                }
                m_size = other.m_size;
                m_end = other.m_end;

                m_ptr = static_cast<T*>(operator new(sizeof(T) * m_size));          // allocate new memory

                for (uint32_t i = 0; i < m_end; i++) {
                    new (&m_ptr[i]) T(other.m_ptr[i]);                              // copy data into memory
                }
            }
            return *this;
        }
        Stack& operator=(Stack&& other) {
            std::cout << "arr_stack::Stack move assignment operator called\n";
            if (this != &other) {
                if (m_ptr) {
                    for (uint32_t i = 0; i < m_size; i++) {
                        m_ptr[i].~T();
                    }
                    operator delete(m_ptr);
                }
                m_size = other.m_size;
                m_end = other.m_end;
                m_ptr = other.m_ptr;                                            // assume ownership of memory

                other.m_end = nullptr;                                          // strip other of ownership
                other.m_size = 0;
                other.m_end = 0;
            }
            return *this;
        }
        void add(T data) {
            std::cout << "Adding " << data << "\n";
            if (m_end == m_size) { resize(m_size * 2); }
            new (&m_ptr[m_end++]) T(std::move(data));
        }
        T pop() {
            std::cout << "Popping from stack\n";
            if (m_end <= 0) {
                throw std::out_of_range("stack.pop() from an empty stack\n");
            }
            T data = std::move(m_ptr[--m_end]);
            m_ptr[m_end].~T();
            if (m_size > 1 && (float)(m_end) / m_size <= 0.25) { resize(m_size / 2); }
            return data;
        }
        void print() {
            std::cout << "Elements: " << (m_end) << " size: " << m_size << "\n";
            std::cout << "[";
            uint32_t i = m_end;
            while (i) {
                std::cout << m_ptr[--i];
                if (i) { std::cout << ", "; }
            }
            std::cout << "]\n";
        }
        friend std::ostream& operator<<(std::ostream& out, const Stack<T>& stack) {
            out << "Elements: " << (stack.m_end) << " size: " << stack.m_size << "\n";
            out << "[";
            uint32_t i = stack.m_end;
            while (i) {
                out << stack.m_ptr[--i];
                if (i) { out << ", "; }
            }
            out << "]\n";
            return out;
        }
        ~Stack() {
            std::cout << "arr_stack::Stack destructor called\n";
            for (uint32_t i = 0; i < m_end; i++) {
                m_ptr[i].~T();
            }
            operator delete(m_ptr);
        }

    private:
        void resize(uint32_t newSize) {
            std::cout << "Resizing stack from: " << m_size << " to: " << newSize << "\n";
            T* tmp = static_cast<T*>(operator new(sizeof(T) * newSize));

            for (uint32_t i = 0; i < m_end; i++) {
                new (&tmp[i]) T(std::move(m_ptr[i]));
                m_ptr[i].~T();
            }

            operator delete(m_ptr);

            m_size = newSize;

            m_ptr = tmp;
            tmp = nullptr;

        }
};

}

#endif