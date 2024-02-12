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

        bool empty();
        std::size_t size() const;
    private:
        std::size_t size_ {0};
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
    bool vector<T>::empty()
    {
        return true;
    }

    template <typename T>
    std::size_t vector<T>::size() const
    {
        return size_;
    }
}


#endif /* VECTOR_HPP */
