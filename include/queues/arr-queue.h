#ifndef ARRAY_QUEUE_H_
#define ARRAY_QUEUE_H_

#include <iostream>
#include <stdexcept>

namespace arr_queue {

template <typename T> class Queue {

    private:
        T* m_ptr{nullptr};                  // Pointer to the start of the array
        unsigned int m_size{1};             // Maximum number of elements that can be added currently
        unsigned int m_elements{0};         // Number of elements currently in the queue
        unsigned int m_start{0};            // Index of the first element
        unsigned int m_end{0};              // Index of the first empty slot in the queue

    public:
        // Default constructor
        Queue() {
            std::cout << "Default constructor\n";
            m_ptr = static_cast<T*>(operator new(sizeof(T) * m_size));      // assign new raw block of memory
        }
        // Copy constructor
        Queue(const Queue<T>& other) {
            std::cout << "Copy constructor\n";
            m_size = other.m_size;
            m_elements = other.m_elements;
            m_start = 0;
            m_end = other.m_elements;

            m_ptr = static_cast<T*>(operator new(sizeof(T) * m_size));      // assign new raw block of memory

            unsigned int index;
            for (unsigned int i = 0; i < m_elements; i++) {
                index = (other.m_start + i) % m_size;
                new (&m_ptr[i]) T(other.m_ptr[index]);                      // copy data into memory 
            }
        }
        // Move constructor
        Queue(Queue<T>&& other) {
            std::cout << "Move constructor\n";
            m_size = other.m_size;
            m_elements = other.m_elements;
            m_start = other.m_start;
            m_end = other.m_end;

            m_ptr = other.m_ptr;                                            // Assume ownership of the memory
            other.m_ptr = nullptr;                                          // Strip ownership from other
            other.m_size = 0;
            other.m_elements = 0;
        }
        // Assignment operator
        Queue<T>& operator=(const Queue<T>& other) {
            std::cout << "Assignment operator=\n";
            if (this != &other) {                                           // prevent self assignment
                // free old queue
                if (m_ptr) {                                                // free old queue (if any)
                    for (unsigned int i; i < m_elements; i++) {
                        m_ptr[i].~T();
                    }
                    
                    operator delete(m_ptr);
                }

                m_size = other.m_size;
                m_elements = other.m_elements;
                m_start = 0;
                m_end = other.m_elements;

                m_ptr = static_cast<T*>(operator new(sizeof(T) * m_size));  // assign new raw block of memory

                unsigned int index;
                for (unsigned int i = 0; i < m_elements; i++) {
                    index = (other.m_start + i) % other.m_size;
                    new (&m_ptr[i]) T(other.m_ptr[index]);                  // copy data into memory
                }
            }
            return *this;
        }
        // Move operator
        Queue<T>& operator=(Queue<T>&& other) {
            std::cout << "Move operator= \n";
            if (this != &other) {                                           // prevent self assignment
                // free old queue
                if (m_ptr) {
                    for (unsigned int i; i < m_elements; i++) {             // free old memory
                        m_ptr[i].~T();
                    }
                    
                    operator delete(m_ptr);
                }

                m_size = other.m_size;
                m_elements = other.m_elements;
                m_start = other.m_start;
                m_end = other.m_end;

                m_ptr = other.m_ptr;                                        // Assume ownership of the memory
                other.m_ptr = nullptr;                                      // Strip other of ownership
                other.m_size = 0;
                other.m_elements = 0;
            }
            return *this;
        }
        // Add item to the queue
        void add(T data) {
            std::cout << "Adding: " << data << "\n";
            if (m_elements == m_size) { resize(m_size * 2); }               // Allocate more memory if required        
            new (&m_ptr[m_end]) T(std::move(data));                         // move data into memory
            m_elements++;
            m_end = (m_end + 1) % m_size;                                   // ensure m_end is in bounds
        }
        // Pop first item from the queue
        T pop() {
            if (m_elements <= 0) {                                          // Check there is an element to pop
                throw std::out_of_range("queue.pop() from an empty queue\n");
            }
            T data = std::move(m_ptr[m_start]);                             // Get head
            m_ptr[m_start].~T();                                            // Clean (old) head
            m_elements--;
            m_start = (m_start + 1) % m_size;                               // ensure m_start is in bounds
            if (m_size > 1 && (float)m_elements / m_size <= 0.25) { resize(m_size / 2); } // resize if needed
            return data;
        }
        // Print out the queue
        void print() {
            std::cout << "Start: " << m_start << " End: " << m_end << " Size: " << m_size << " Elements: " << m_elements << " Capacity: " << (float)m_elements / m_size << "\n";
            std::cout << "[";
            for (unsigned int i = 0; i < m_elements; i++) {
                std::cout << m_ptr[(m_start + i) % m_size];
                if (i != (m_elements - 1)) { std::cout << ", ";}
            }
            std::cout << "]\n";
        }
        // Destructor
        ~Queue() {
            std::cout << "\nDestructor called\n";
            unsigned int index;
            for (unsigned int i = 0; i < m_elements; i++) {                 // destroy each element
                index = (m_start + i) % m_size;
                m_ptr[index].~T();
            }

            operator delete(m_ptr);                                         // free memory block

            std::cout << "Deleted\n";
        }

    private:
        // resize queue to a new given size
        void resize(unsigned int newSize) {
            std::cout << "Resizing aray from: " << m_size << " to: " << newSize << "\n";
            T* tmp = static_cast<T*>(operator new(sizeof(T) * newSize));    // new queue

            unsigned int index;
            for (unsigned int i = 0; i < m_elements; i++) {
                index = (m_start + i) % m_size;
                new (&tmp[i]) T(std::move(m_ptr[index]));                   // steal old data
                m_ptr[index].~T();
            }

            m_size = newSize;
            m_start = 0;
            m_end = m_elements;

            operator delete(m_ptr);                                         // delete old queue

            m_ptr = tmp;                                                    // make m_ptr point to temp
            tmp = nullptr;
            std::cout << "Resized\n";
        }
};

}

#endif