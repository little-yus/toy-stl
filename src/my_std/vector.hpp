#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <memory>
#include <stdexcept>

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

        vector(vector&& other) = default;
        vector& operator= (vector&& other) = default;

        bool empty();
        std::size_t size() const;

        T& operator[] (std::size_t index);
        const T& operator[] (std::size_t index) const;

        T& at(std::size_t index);
        const T& at(std::size_t index) const;

        T& front();
        const T& front() const;

        T& back();
        const T& back() const;

    private:
        std::size_t size_ {0};
        std::unique_ptr<T[]> data_ {nullptr};
    };

    // Constructors
    template <typename T>
    vector<T>::vector()
    {

    }

    template <typename T>
    vector<T>::vector(std::size_t size) : size_{size}, data_{std::make_unique<T[]>(size)}
    {

    }

    // Note:
    // When you use new[] each element is initialized by the default constructor except when the type is a built-in type. 
    // Built-in types are left unitialized by default.
    template <typename T>
    vector<T>::vector(std::size_t size, const T& value) : size_{size}, data_{std::make_unique<T[]>(size)}
    {
        T* begin = data_.get();
        T* end = data_.get() + size;

        for (T* ptr = begin; ptr != end; ptr += 1) {
            *ptr = value;
        }
    }

    template <typename T>
    vector<T>::vector(const vector& other) : size_{other.size_}, data_{std::make_unique<T[]>(other.size_)}
    {
        for (std::size_t i = 0; i < size_; i += 1) {
            data_[i] = other.data_[i];
        }
    }

    template <typename T>
    vector<T>& vector<T>::operator= (vector other)
    {
        std::swap(*this, other);
        return *this;
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
}


#endif /* VECTOR_HPP */
