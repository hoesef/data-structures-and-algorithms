#ifndef ARRAY_STACK_H_
#define ARRAY_STACK_H_

namespace arr_stack {

template <typename T> class Stack {

    private:
        T* m_ptr{nullptr};
        unsigned int m_size{1};
        unsigned int m_end{0};

    public:
        Stack() {
            std::cout << "Default constructor\n";
            m_ptr = static_cast<T*>(operator new(sizeof(T) * m_size));
        }
        Stack(const Stack& other) {
            std::cout << "Copy constructor\n";
            m_size = other.m_size;
            m_end = other.m_end;

            m_ptr = static_cast<T*>(operator new(sizeof(T) * m_size));

            // for (unsigned int i; i < m_end; i++) {
            //     new (&m_ptr[i]) T(std::move())
            // }
        }
        ~Stack() {
            for (unsigned int i = 0; i < m_end; i++) {
                m_ptr[i].~T();
            }
            operator delete(m_ptr);
        }

    private:
        // void resize() {}

};

}

#endif