#ifndef UNIQUE_POINRER_H_
#define UNIQUE_POINTER_H_

namespace smrt_ptr {
 
template <typename T> class Unique_pointer {

    private:
        T* m_ptr{nullptr};

    public:
        Unique_pointer(T* p = nullptr): m_ptr{p} {}
        Unique_pointer(const Unique_pointer& other) = delete;
        Unique_pointer(Unique_pointer<T>&& other) {
            m_ptr(other.m_ptr);
            other.m_ptr = nullptr;
        }
        Unique_pointer<T>& operator=(const Unique_pointer<T>& other) = delete;
        Unique_pointer<T>& operator=(Unique_pointer<T>&& other) {
            m_ptr = other.m_ptr;
            other.m_ptr = nullptr;
        }
        T* release() {
            T* ptr = m_ptr;
            m_ptr = nullptr;
            return ptr;
        }
        void reset() {
            delete m_ptr;
            m_ptr = nullptr;
        }
        void swap(Unique_pointer<T>& other) {
            T* tmp = m_ptr;
            m_ptr = other.m_ptr;
            other.m_ptr = tmp; 
        }
        T* get() {
            return m_ptr;
        }
        bool operator_bool() {
            return ((m_ptr) ? true:false);
        }
        T& operator*() {
            return *m_ptr;
        }
        T* operator->() {
            return m_ptr;
        }
        ~Unique_pointer() { 
            delete m_ptr;
            m_ptr = nullptr;
         }

};

// make_unique
template <typename T, typename... Args> Unique_pointer<T> make_unique(Args&&... args) {
    return Unique_pointer(new T(std::forward<Args>(args)...));
}

}

#endif