#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <memory>

namespace my
{
    template <typename T>
    class vector
    {
    public:
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
}


#endif /* VECTOR_HPP */
