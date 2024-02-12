#ifndef VECTOR_HPP
#define VECTOR_HPP

namespace my
{
    template <typename T>
    class vector
    {
    public:
        vector();
        vector(std::size_t size);
        vector(std::size_t size, const T& value);

        bool empty();
        std::size_t size() const;
        T& operator[] (std::size_t index);
        const T& operator[] (std::size_t index) const;
    private:
        std::size_t size_ {0};
        T data {0}; // This is not enough now
    };

    template <typename T>
    vector<T>::vector()
    {

    }

    template <typename T>
    vector<T>::vector(std::size_t size) : size_{size}
    {

    }

    template <typename T>
    vector<T>::vector(std::size_t size, const T& value) : size_{size}, data{value}
    {

    }

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
        return data;
    }

    template <typename T>
    const T& vector<T>::operator[] (std::size_t index) const
    {
        return data;
    }
}


#endif /* VECTOR_HPP */
