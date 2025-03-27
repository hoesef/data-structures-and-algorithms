#ifndef SHARED_POINTER_H_
#define SHARED_POINTER_H_

namespace smrt_ptr {

template <typename T> class Shared_pointer {

    private:
        int* m_ref_count{nullptr};
        T* m_ptr{nullptr};

    public:
        Shared_pointer() {}
        Shared_pointer(T* ptr) {
            m_ref_count = new int(1);
            m_ptr = ptr;
        }
        Shared_pointer(const Shared_pointer<T>& other) {
            m_ref_count = other.m_ref_count;
            m_ptr = other.m_ptr;
            (*m_ref_count)++;
        }
        Shared_pointer(Shared_pointer<T>&& other) {
            m_ref_count = other.m_ref_count;
            m_ptr = other.m_ptr;
            (*m_ref_count)++;
        }
        Shared_pointer<T>& operator=(const Shared_pointer<T> other) {
            if (this != &other) {
                m_ref_count = other.m_ref_count;
                m_ptr = other.m_ptr;
                (*m_ref_count)++;
            }
            return *this;
        }
        Shared_pointer<T>& operator=(Shared_pointer<T>&& other) {
            if (this != &other) {
                m_ref_count = other.m_ref_count;
                m_ptr = other,m_ptr;
                (*m_ref_count)++;
            }
            return *this;
        }
        void reset() {
            if (!m_ref_count) { return; }
            if (*m_ref_count == 1) {
                delete m_ptr;
                delete m_ref_count;
                m_ptr = nullptr;
                m_ref_count = nullptr;
            } else {
                (*m_ref_count)--;
                m_ptr = nullptr;
                m_ref_count = nullptr;
            }
        }
        void reset(T* ptr) {
            if (!m_ref_count) {
                m_ref_count = new int(1);
                m_ptr = ptr;
            } else if (*m_ref_count == 1) {
                delete m_ptr;
                m_ptr = ptr;
            } else {
                (*m_ref_count)--;
                m_ptr = ptr;
                m_ref_count = new int(1);
            }
        }
        T* get() {
            return m_ptr;
        }
        void swap(Shared_pointer<T>& other) {
            T* tmp_ptr = m_ptr;
            m_ptr = other.m_ptr;
            other.m_ptr = tmp_ptr;

            int* tmp_ref = m_ref_count;
            m_ref_count = other.m_ref_count;
            other.m_ref_count = tmp_ref;
        }
        T& operator*() {
            return *m_ptr;
        }
        T* operator->() {
            return m_ptr;
        }
        int use_count() {
            if (!m_ref_count) {return 0;}
            return *m_ref_count;
        }
        bool unique() {
            return (use_count() <= 1);
        }
        bool operator_bool() {
            return ((m_ptr)?true:false);
        }
        ~Shared_pointer() {
            if (!m_ref_count) {return;}             // Check we are tracking something

            if ((*m_ref_count) <= 1) {                // If last reference, delete 
                delete m_ptr;
                delete m_ref_count;
                m_ptr = nullptr;
                m_ref_count = nullptr;
            } else {                                // else decrease ref count
                (*m_ref_count)--;
            }
        }

};

template <typename T, typename... Args> Shared_pointer<T> make_shared(Args&&... args) {
    return Shared_pointer(new T(std::forward<Args>(args)...));
}

}

#endif