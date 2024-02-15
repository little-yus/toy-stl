#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <stdexcept>
#include <new>

namespace my
{
    template <typename T>
    class vector
    {
    public:
        using value_type = T;
        using reference = T&;
        using const_reference = const T&;
        
        using size_type = std::size_t;

        vector();
        vector(std::size_t size);
        vector(std::size_t size, const T& value);

        vector(const vector& other);
        vector& operator= (vector other);

        vector(vector&& other);
        vector& operator= (vector&& other);

        ~vector();

        void swap(vector& other);

        bool empty();
        std::size_t size() const;
        std::size_t capacity() const;

        // Accessors
        T& operator[] (std::size_t index);
        const T& operator[] (std::size_t index) const;

        T& at(std::size_t index);
        const T& at(std::size_t index) const;

        T& front();
        const T& front() const;

        T& back();
        const T& back() const;

        // Modifiers
        void push_back(const T& value);
        void push_back(T&& value);
        void pop_back();

    private:
        std::size_t size_ {0};
        std::size_t capacity_ {0};
        T* data_ {nullptr};
    };

    // Constructors
    template <typename T>
    vector<T>::vector()
    {

    }

    template <typename T>
    vector<T>::vector(std::size_t size) :
        size_{size},
        capacity_{size},
        data_{static_cast<T*>(::operator new (sizeof(T) * size))}
    {
        T* begin = data_;
        T* end = data_ + size;

        for (T* ptr = begin; ptr != end; ptr += 1) {
            new (ptr) T();
        }
    }

    // Note:
    // When you use new[] each element is initialized by the default constructor except when the type is a built-in type. 
    // Built-in types are left unitialized by default.
    template <typename T>
    vector<T>::vector(std::size_t size, const T& value) :
        size_{size},
        capacity_{size},
        data_{static_cast<T*>(::operator new (sizeof(T) * size))}
    {
        T* begin = data_;
        T* end = data_ + size;

        for (T* ptr = begin; ptr != end; ptr += 1) {
            new (ptr) T(value);
        }
    }

    template <typename T>
    vector<T>::vector(const vector& other) :
        size_{other.size_},
        capacity_{other.size_}, // vector does not have to copy capacity
        data_{static_cast<T*>(::operator new (sizeof(T) * other.size_))}
    {
        for (std::size_t i = 0; i < size_; i += 1) {
            new (data_ + i) T(other.data_[i]);
        }
    }

    // Maybe reimplement for better performance?
    template <typename T>
    vector<T>& vector<T>::operator= (vector other)
    {
        this->swap(other);
        return *this;
    }

    template <typename T>
    vector<T>::vector(vector&& other) :
        size_{other.size_},
        capacity_{other.capacity_},
        data_{other.data_}
    {
        other.size_ = 0;
        other.capacity_ = 0;
        other.data_ = nullptr;
    }

    template <typename T>
    vector<T>& vector<T>::operator= (vector&& other)
    {
        this->swap(vector<T>(std::move(other)));
        return *this;
    }

    template <typename T>
    vector<T>::~vector()
    {
        T* begin = data_;
        T* end = data_ + size_;

        for (T* ptr = begin; ptr != end; ptr += 1) {
            ptr->~T();
        }

        ::operator delete(static_cast<void*>(data_)); // Not sure if this is ok
    }

    template <typename T>
    void vector<T>::swap(vector& other)
    {
        using std::swap;
        swap(this->size_, other.size_);
        swap(this->capacity_, other.capacity_);
        swap(this->data_, other.data_);
    }

    // Other functions
    template <typename T>
    bool vector<T>::empty()
    {
        return true;
    }

    template <typename T>
    std::size_t vector<T>::size() const
    {
        return size_;
    }

    template <typename T>
    std::size_t vector<T>::capacity() const
    {
        return capacity_;
    }

    template <typename T>
    T& vector<T>::operator[] (std::size_t index)
    {
        return data_[index];
    }

    template <typename T>
    const T& vector<T>::operator[] (std::size_t index) const
    {
        return data_[index];
    }

    template <typename T>
    T& vector<T>::at(std::size_t index)
    {
        if (index >= size()) {
            throw std::out_of_range("Invalid element index");
        }

        return (*this)[index];
    }

    template <typename T>
    const T& vector<T>::at(std::size_t index) const
    {
        if (index >= size()) {
            throw std::out_of_range("Invalid element index");
        }

        return (*this)[index];
    }

    template <typename T>
    T& vector<T>::front()
    {
        return (*this)[0];
    }

    template <typename T>
    const T& vector<T>::front() const
    {
        return (*this)[0];
    }

    template <typename T>
    T& vector<T>::back()
    {
        return (*this)[size() - 1];
    }

    template <typename T>
    const T& vector<T>::back() const
    {
        return (*this)[size() - 1];
    }

    template <typename T>
    void vector<T>::push_back(const T& value)
    {

    }

    template <typename T>
    void vector<T>::push_back(T&& value)
    {

    }

    template <typename T>
    void vector<T>::pop_back()
    {
        size_ -= 1;
        data_[size_].~T();
    }
}


#endif /* VECTOR_HPP */
