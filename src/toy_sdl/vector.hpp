#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "vector_iterator.hpp"
#include "vector_const_iterator.hpp"
#include "iterator.hpp"

#include <stdexcept>
#include <new>
#include <cassert>
#include <memory>
#include <cmath>

namespace my
{
    template <typename T, typename A = std::allocator<T>>
    class vector
    {
    public:
        // Member types
        using value_type = T;

        using reference = T&;
        using const_reference = const T&;

        using pointer = T*;
        using const_pointer = const T*;
        
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

        using iterator = vector_iterator<T>;
        using const_iterator = vector_const_iterator<T>;

        using reverse_iterator = my::reverse_iterator<iterator>; // Here we shadow reverse_iterator class template, be careful
        using const_reverse_iterator = my::reverse_iterator<const_iterator>;

        using allocator_type = A;

        // Constructors
        constexpr vector() noexcept(noexcept(A()));
        constexpr explicit vector(const A& allocator) noexcept;
        constexpr vector(size_type size, const A& allocator = A());
        constexpr explicit vector(size_type size, const T& value, const A& allocator = A());
        template <std::input_iterator I>
        constexpr vector(I begin, I end, const A& allocator = A());
        constexpr vector(std::initializer_list<T> init_list, const A& allocator = A());

        // Rule of 5
        constexpr vector(const vector& other);
        constexpr vector& operator= (vector other);
        constexpr vector(vector&& other) noexcept;
        constexpr vector& operator= (vector&& other);
        ~vector();

        // Other
        constexpr vector& operator= (std::initializer_list<T> init_list);
        constexpr void swap(vector& other) noexcept(
            std::allocator_traits<allocator_type>::propagate_on_container_swap::value || std::allocator_traits<allocator_type>::is_always_equal::value
        );

        // Propery access
        constexpr bool empty() const noexcept;
        constexpr size_type size() const noexcept;
        constexpr size_type max_size() const noexcept;
        constexpr size_type capacity() const noexcept;
        constexpr T* data() noexcept;
        constexpr const T* data() const noexcept;

        // Comparison
        constexpr bool operator== (const vector& other) const;
        constexpr auto operator<=> (const vector& other) const;

        // Element access
        constexpr reference operator[] (size_type index);
        constexpr const_reference operator[] (size_type index) const;
        constexpr reference at(size_type index);
        constexpr const_reference at(size_type index) const;
        constexpr reference front();
        constexpr const_reference front() const;
        constexpr reference back();
        constexpr const_reference back() const;

        // Adding/removing elements
        constexpr void push_back(const T& value);
        constexpr void push_back(T&& value);
        constexpr void pop_back(); // not noexcept because reasons https://stackoverflow.com/questions/55222295/when-can-stdvectorpop-back-throw-an-exception
        template <typename ... Args>
        constexpr reference emplace_back(Args&& ... args);
        constexpr iterator insert(const_iterator pos, const T& value);
        constexpr iterator insert(const_iterator pos, T&& value);
        constexpr iterator insert(const_iterator pos, size_type count, const T& value);
        template <std::random_access_iterator InputIt>
        constexpr iterator insert(const_iterator pos, InputIt first, InputIt last);
        constexpr iterator insert(const_iterator pos, std::initializer_list<T> init_list);
        constexpr iterator erase(const_iterator pos);
        template <typename ... Args>
        constexpr iterator emplace(const_iterator pos, Args&& ... args);

        // Size/capacity modification
        constexpr void clear() noexcept;
        constexpr void resize(size_type new_size);
        constexpr void resize(size_type new_size, const T& value);
        constexpr void reserve(size_type new_capacity);
        constexpr void shrink_to_fit();

        // Iterators
        constexpr iterator begin() noexcept;
        constexpr iterator end() noexcept;
        constexpr const_iterator begin() const noexcept;
        constexpr const_iterator end() const noexcept;
        constexpr const_iterator cbegin() const noexcept;
        constexpr const_iterator cend() const noexcept;

        // Reverse iterators
        constexpr reverse_iterator rbegin() noexcept;
        constexpr reverse_iterator rend() noexcept;
        constexpr const_reverse_iterator rbegin() const noexcept;
        constexpr const_reverse_iterator rend() const noexcept;
        constexpr const_reverse_iterator crbegin() const noexcept;
        constexpr const_reverse_iterator crend() const noexcept;

    private:
        bool is_memory_filled() const;
        size_type calculate_new_capacity() const;
        void grow();

        // TODO: Rename these, cause now things operate on 2 ranges vs 1 range and value have similar names
        constexpr void destroy_range(pointer begin, pointer end);
        constexpr void default_construct_range(pointer begin, pointer end);
        constexpr void copy_construct_range(pointer begin, pointer end, const T& value);
        template <typename InputIt, typename OutputIt>
        constexpr void copy_range(InputIt source_begin, InputIt source_end, OutputIt destination_begin);
        constexpr void move_range(pointer source_begin, pointer source_end, pointer destination_begin);
        constexpr void copy_range_backwards(pointer source_begin, pointer source_end, pointer destination_end);
        constexpr void move_range_backwards(pointer source_begin, pointer source_end, pointer destination_end);
        constexpr void copy_assign_range(pointer begin, pointer end, const T& value);
        template <typename InputIt, typename OutputIt>
        constexpr void copy_assign_range_to_range(InputIt source_begin, InputIt source_end, OutputIt destination_begin);
        constexpr void move_assign_range_backwards(pointer source_begin, pointer source_end, pointer destination_end);
        constexpr void move_assign_range(pointer source_begin, pointer source_end, pointer destination_begin);

        A allocator_ { };
        size_type size_ {0};
        size_type capacity_ {0};
        T* data_ {nullptr};
    };

    // Constructors
    template <typename T, typename A>
    constexpr vector<T, A>::vector() noexcept(noexcept(A()))
    {

    }

    // Constructors
    template <typename T, typename A>
    constexpr vector<T, A>::vector(const A& allocator) noexcept : allocator_{allocator}
    {

    }

    template <typename T, typename A>
    constexpr vector<T, A>::vector(size_type size, const A& allocator) :
        allocator_{allocator},
        size_{size},
        capacity_{size},
        data_{std::allocator_traits<A>::allocate(allocator_, size)}
    {
        default_construct_range(data_, data_ + size_);
    }

    template <typename T, typename A>
    constexpr vector<T, A>::vector(size_type size, const T& value, const A& allocator) :
        allocator_{allocator},
        size_{size},
        capacity_{size},
        data_{std::allocator_traits<A>::allocate(allocator_, size)}
    {
        copy_construct_range(data_, data_ + size_, value);
    }

    template <typename T, typename A>
    template <std::input_iterator I>
    constexpr vector<T, A>::vector(I first, I last, const A& allocator) : vector(allocator)
    {
        for (auto i = first; i != last; ++i) {
            push_back(*i);
        }
    }

    template <typename T, typename A>
    constexpr vector<T, A>::vector(std::initializer_list<T> init_list, const A& allocator) :
        vector(std::begin(init_list), std::end(init_list), allocator)
    {

    }

    // Rule of 5
    template <typename T, typename A>
    constexpr vector<T, A>::vector(const vector& other) :
        allocator_{std::allocator_traits<A>::select_on_container_copy_construction(other.allocator_)},
        size_{other.size_},
        capacity_{other.size_}, // vector does not have to copy capacity
        data_{std::allocator_traits<A>::allocate(allocator_, other.size_)}
    {
        copy_range(other.data_, other.data_ + other.size_, data_);
    }

    // Maybe reimplement for better performance?
    template <typename T, typename A>
    constexpr vector<T, A>& vector<T, A>::operator= (vector other)
    {
        this->swap(other);
        return *this;
    }

    template <typename T, typename A>
    constexpr vector<T, A>::vector(vector&& other) noexcept :
        allocator_{std::move(other.allocator_)},
        size_{other.size_},
        capacity_{other.capacity_},
        data_{other.data_}
    {
        other.size_ = 0;
        other.capacity_ = 0;
        other.data_ = nullptr;
    }

    template <typename T, typename A>
    constexpr vector<T, A>& vector<T, A>::operator= (vector&& other)
    {
        this->swap(vector<T, A>(std::move(other)));
        return *this;
    }

    template <typename T, typename A>
    vector<T, A>::~vector()
    {
        destroy_range(data_, data_ + size_);
        std::allocator_traits<A>::deallocate(allocator_, data_, capacity_);
        // zeroing member variables is not required, so data_, size_ and capacity_ still contain garbage
    }

    template <typename T, typename A>
    constexpr vector<T, A>& vector<T, A>::operator= (std::initializer_list<T> init_list)
    {
        vector<T, A> temporary(init_list);
        this->swap(temporary);
        return *this;
    }

    template <typename T, typename A>
    constexpr void vector<T, A>::swap(vector& other) noexcept(
        std::allocator_traits<allocator_type>::propagate_on_container_swap::value || std::allocator_traits<allocator_type>::is_always_equal::value
    )
    {
        using std::swap;
        if (std::allocator_traits<allocator_type>::propagate_on_container_swap::value) {
            swap(this->allocator_, other.allocator_);
        }

        swap(this->size_, other.size_);
        swap(this->capacity_, other.capacity_);
        swap(this->data_, other.data_);
    }
    
    // Propery access
    template <typename T, typename A>
    constexpr bool vector<T, A>::empty() const noexcept
    {
        return size_ == 0;
    }
    
    template <typename T, typename A>
    constexpr vector<T, A>::size_type vector<T, A>::size() const noexcept
    {
        return size_;
    }

    template <typename T, typename A>
    constexpr vector<T, A>::size_type vector<T, A>::max_size() const noexcept
    {
        return std::numeric_limits<difference_type>::max();
    }
    
    template <typename T, typename A>
    constexpr vector<T, A>::size_type vector<T, A>::capacity() const noexcept
    {
        return capacity_;
    }

    template <typename T, typename A>
    constexpr T* vector<T, A>::data() noexcept
    {
        return data_;
    }

    template <typename T, typename A>
    constexpr const T* vector<T, A>::data() const noexcept
    {
        return data_;
    }

    // Comparisons
    template <typename T, typename A>
    constexpr bool vector<T, A>::operator== (const vector& other) const
    {
        if (size() != other.size()) {
            return false;
        }

        for (size_type i = 0; i < size(); i += 1) {
            if ((*this)[i] != other[i]) {
                return false;
            }
        }

        return true;
    }

    template <typename T, typename A>
    constexpr auto vector<T, A>::operator<=> (const vector& other) const
    {
        if constexpr (std::three_way_comparable<T>) {
            for (size_type i = 0; i < std::min(size(), other.size()); i += 1) {
                if (auto comparison_result = (*this)[i] <=> other[i]; comparison_result != 0) {
                    return comparison_result;
                }
            }

            using ordering = std::compare_three_way_result_t<T>;
            return static_cast<ordering>(size() <=> other.size());
        } else {
            // If value_type does not implement <=>, but has == and < then vector can still be ordered
            static_assert(requires (const T& a, const T& b)
            {
                { a == b } -> std::convertible_to<bool>;
                { a < b } -> std::convertible_to<bool>;
            });

            for (size_type i = 0; i < std::min(size(), other.size()); i += 1) {
                if ((*this)[i] < other[i]) {
                    return std::weak_ordering::less;
                }

                if (other[i] < (*this)[i]) {
                    return std::weak_ordering::greater;
                }
            }

            return std::weak_order(size(), other.size());
        }
    }

    // Element access
    template <typename T, typename A>
    constexpr vector<T, A>::reference vector<T, A>::operator[] (size_type index)
    {
        return data_[index];
    }

    template <typename T, typename A>
    constexpr vector<T, A>::const_reference vector<T, A>::operator[] (size_type index) const
    {
        return data_[index];
    }

    template <typename T, typename A>
    constexpr vector<T, A>::reference vector<T, A>::at(size_type index)
    {
        if (index >= size()) {
            throw std::out_of_range("Invalid element index");
        }

        return (*this)[index];
    }

    template <typename T, typename A>
    constexpr vector<T, A>::const_reference vector<T, A>::at(size_type index) const
    {
        if (index >= size()) {
            throw std::out_of_range("Invalid element index");
        }

        return (*this)[index];
    }

    template <typename T, typename A>
    constexpr vector<T, A>::reference vector<T, A>::front()
    {
        return (*this)[0];
    }

    template <typename T, typename A>
    constexpr vector<T, A>::const_reference vector<T, A>::front() const
    {
        return (*this)[0];
    }

    template <typename T, typename A>
    constexpr vector<T, A>::reference vector<T, A>::back()
    {
        return (*this)[size() - 1];
    }

    template <typename T, typename A>
    constexpr vector<T, A>::const_reference vector<T, A>::back() const
    {
        return (*this)[size() - 1];
    }

    // Adding/removing elements
    template <typename T, typename A>
    constexpr void vector<T, A>::push_back(const T& value)
    {
        emplace_back(value);
    }  

    template <typename T, typename A>
    constexpr void vector<T, A>::push_back(T&& value)
    {
        emplace_back(std::move(value));
    }

    template <typename T, typename A>
    constexpr void vector<T, A>::pop_back()
    {
        size_ -= 1;
        std::allocator_traits<A>::destroy(allocator_, data_ + size_);
    }

    template <typename T, typename A>
    template <typename ... Args>
    constexpr vector<T, A>::reference vector<T, A>::emplace_back(Args&& ... args)
    {
        if (is_memory_filled()) {
            grow();
        }
        
        std::allocator_traits<A>::construct(allocator_, data_ + size_, std::forward<Args>(args) ...);
        size_ += 1;

        return back();
    }

    template <typename T, typename A>
    constexpr vector<T, A>::iterator vector<T, A>::insert(const_iterator pos, const T& value)
    {
        return emplace(pos, value);
    }

    template <typename T, typename A>
    constexpr vector<T, A>::iterator vector<T, A>::insert(const_iterator pos, T&& value)
    {
        return emplace(pos, std::move(value));
    }

    template <typename T, typename A>
    constexpr vector<T, A>::iterator vector<T, A>::insert(const_iterator pos, size_type count, const T& value)
    {
        const auto insert_position = pos - cbegin();

        if (count == 0) {
            return iterator(data_ + insert_position);
        }

        const auto new_size = size_ + count;

        if (new_size > capacity_) {
            std::size_t new_capacity = new_size; // TODO: Maybe max(new_size, calculate_new_capacity()) ??
            T* new_data = std::allocator_traits<A>::allocate(allocator_, new_capacity);

            // Move elements [begin, pos)
            move_range(data_, data_ + insert_position, new_data);
            // Move elements [pos, end)
            // Elements are move constructed in new location, so this is ok
            move_range_backwards(data_ + insert_position, data_ + size_, new_data + new_size);
            copy_construct_range(new_data + insert_position, new_data + insert_position + count, value);

            // Clear old memory
            destroy_range(data_, data_ + size_);
            std::allocator_traits<A>::deallocate(allocator_, data_, capacity_);

            data_ = new_data;
            capacity_ = new_capacity;
            size_ = new_size;
        } else {
            // Move last count elements to uninitialized memory
            // Backwards because this way arguments are easier to understand
            // TODO: There should be better way than static_cast<difference_type>(count) everywhere
            move_range_backwards(
                data_ + size_ - std::min(cend() - pos, static_cast<difference_type>(count)),
                data_ + size_,
                data_ + size_ + count
            );
            // Move others to the end of array
            // Assignment because destructors were not called for moved-out objects
            // Backwards because input and output ranges can overlap
            move_assign_range_backwards(
                data_ + insert_position,
                data_ + size_ - std::min(cend() - pos, static_cast<difference_type>(count)),
                data_ + size_
            );

            // Some of new elements copy constructed in uninitialized memory
            copy_construct_range(
                data_ + size_,
                data_ + size_ + count - std::min(cend() - pos, static_cast<difference_type>(count)),
                value
            );

            // Some are copy assigned to moved-from objects in already occupied memory
            copy_assign_range(
                data_ + insert_position,
                data_ + insert_position + std::min(cend() - pos, static_cast<difference_type>(count)),
                value
            );

            size_ = new_size;
        }

        return iterator(data_ + insert_position);
    }

    template <typename T, typename A>
    template <std::random_access_iterator InputIt>
    constexpr vector<T, A>::iterator vector<T, A>::insert(const_iterator pos, InputIt first, InputIt last)
    {
        const auto insert_position = pos - cbegin();

        if (first == last) {
            return iterator(data_ + insert_position);
        }

        const auto count = last - first; // FIXME: Does not work for some types of iterators
        const auto new_size = size_ + count;

        if (new_size > capacity_) {
            std::size_t new_capacity = new_size;
            T* new_data = std::allocator_traits<A>::allocate(allocator_, new_capacity);

            // Move elements [begin, pos)
            move_range(data_, data_ + insert_position, new_data);
            // Move elements [pos, end)
            // Elements are move constructed in new location, so this is ok
            move_range_backwards(data_ + insert_position, data_ + size_, new_data + new_size);
            // Copy data from input iterator range to new location
            copy_range(first, last, new_data + insert_position);

            // Clear old memory
            destroy_range(data_, data_ + size_);
            std::allocator_traits<A>::deallocate(allocator_, data_, capacity_);

            data_ = new_data;
            capacity_ = new_capacity;
            size_ = new_size;
        } else {
            // Move last count elements to uninitialized memory
            // Backwards because this way arguments are easier to understand
            // TODO: There should be better way than static_cast<difference_type>(count) everywhere
            move_range_backwards(
                data_ + size_ - std::min(cend() - pos, static_cast<difference_type>(count)),
                data_ + size_,
                data_ + size_ + count
            );
            // Move others to the end of array
            // Assignment because destructors were not called for moved-out objects
            // Backwards because input and output ranges can overlap
            move_assign_range_backwards(
                data_ + insert_position,
                data_ + size_ - std::min(cend() - pos, static_cast<difference_type>(count)),
                data_ + size_
            );

            // Some of new elements copy constructed in uninitialized memory
            // Didn't work without explicit <InputIt, T*>
            // Probably bacause it cannot deduce result type of first + std::min(cend() - pos, static_cast<difference_type>(count))
            // Would by nice to restrict InputIt to concept that has operator+ with int
            copy_range<InputIt, T*>(
                first + std::min(cend() - pos, static_cast<difference_type>(count)),
                last,
                data_ + size_
            );

            // Some are copy assigned to moved-from objects in already occupied memory
            copy_assign_range_to_range<InputIt, T*>(
                first,
                first + std::min(cend() - pos, static_cast<difference_type>(count)),
                data_ + insert_position
            );

            size_ = new_size;
        }

        return iterator(data_ + insert_position);
    }

    template <typename T, typename A>
    constexpr vector<T, A>::iterator vector<T, A>::insert(const_iterator pos, std::initializer_list<T> init_list)
    {
        return insert(pos, std::begin(init_list), std::end(init_list));
    }

    template <typename T, typename A>
    constexpr vector<T, A>::iterator vector<T, A>::erase(const_iterator pos)
    {
        if (empty()) {
            return end();
        } else {
            const auto erase_position = pos - cbegin();
        
            move_assign_range(data_ + erase_position + 1, data_ + size_, data_ + erase_position);
            std::allocator_traits<A>::destroy(allocator_, data_ + size_ - 1);

            --size_;

            return iterator(data_ + erase_position);
        }
    }

    template <typename T, typename A>
    template <typename ... Args>
    constexpr vector<T, A>::iterator vector<T, A>::emplace(const_iterator pos, Args&& ... args)
    {
        const auto new_element_index = pos - cbegin();
        if (is_memory_filled()) {
            std::size_t new_capacity = calculate_new_capacity();
            T* new_data = std::allocator_traits<A>::allocate(allocator_, new_capacity);

            // Move elements [begin, pos)
            move_range(data_, data_ + new_element_index, new_data);
            // Move elements [pos, end)
            // Elements are move constructed in new location, so this is ok
            move_range_backwards(data_ + new_element_index, data_ + size_, new_data + size_ + 1);
            // Construct value in correct position
            std::allocator_traits<A>::construct(allocator_, new_data + new_element_index, std::forward<Args>(args) ...);

            // Clear old memory
            destroy_range(data_, data_ + size_);
            std::allocator_traits<A>::deallocate(allocator_, data_, capacity_);

            data_ = new_data;
            capacity_ = new_capacity;
            size_ = size_ + 1;

            return iterator(new_data + new_element_index);
        } else {
            // Handle case without reallocation
            const auto elements_need_to_be_moved = cend() - pos;
            
            if (elements_need_to_be_moved > 0) {
                // Last element is move constructed in uninitialized memory
                std::allocator_traits<A>::construct(allocator_, data_ + size_, std::move(data_[size_ - 1]));
                // Others are move assigned to next element each in reverse order
                move_assign_range_backwards(data_ + new_element_index, data_ + size_ - 1, data_ + size_);
            }
            // Copy assign value to moved from object at new_element_index
            // TODO: What to do if pos == end and data_ + new_element_index will be in uninit memory??
            *(data_ + new_element_index) = std::move(T(std::forward<Args>(args) ...)); // TODO: Replace with destructor and constructor maybe

            size_ = size_ + 1;

            return iterator(data_ + new_element_index);
        }
    }

    // Size/capacity modification
    template <typename T, typename A>
    constexpr void vector<T, A>::clear() noexcept
    {
        destroy_range(data_, data_ + size_);
        size_ = 0;
    }

    template <typename T, typename A>
    constexpr void vector<T, A>::resize(size_type new_size)
    {
        if (new_size > size()) {
            if (new_size > capacity()) {
                // Move data to new memory location
                const std::size_t new_capacity = new_size;
                T* new_data = std::allocator_traits<A>::allocate(allocator_, new_capacity);
                move_range(data_, data_ + size_, new_data);

                // Clear old memory
                destroy_range(data_, data_ + size_);
                std::allocator_traits<A>::deallocate(allocator_, data_, capacity_);

                data_ = new_data;
                capacity_ = new_capacity;
            }

            // Default construct new elements
            default_construct_range(data_ + size_, data_ + new_size);
            size_ = new_size;
        } else if (new_size < size()) {
            destroy_range(data_ + new_size, data_ + size_);
            size_ = new_size;
        }
    }

    template <typename T, typename A>
    constexpr void vector<T, A>::resize(size_type new_size, const T& value)
    {
        if (new_size > size()) {
            if (new_size > capacity()) {
                // Move data to new memory location
                const std::size_t new_capacity = new_size;

                T* new_data = std::allocator_traits<A>::allocate(allocator_, new_capacity);
                move_range(data_, data_ + size_, new_data);

                // Clear old memory
                destroy_range(data_, data_ + size_);
                std::allocator_traits<A>::deallocate(allocator_, data_, capacity_);

                data_ = new_data;
                capacity_ = new_capacity;
            }

            // Copy construct new elements
            copy_construct_range(data_ + size_, data_ + new_size, value);
            size_ = new_size;
        } else if (new_size < size()) {
            destroy_range(data_ + new_size, data_ + size_);
            size_ = new_size;
        }
    }

    template <typename T, typename A>
    constexpr void vector<T, A>::reserve(size_type new_capacity)
    {
        if (new_capacity > capacity()) {
            // Move data to new memory location
            T* new_data = std::allocator_traits<A>::allocate(allocator_, new_capacity);
            move_range(data_, data_ + size_, new_data);

            // Clear old memory
            destroy_range(data_, data_ + size_);
            std::allocator_traits<A>::deallocate(allocator_, data_, capacity_);

            data_ = new_data;
            capacity_ = new_capacity;
        }
    }

    template <typename T, typename A>
    constexpr void vector<T, A>::shrink_to_fit()
    {
        if (size() < capacity()) {
            // Move data to new memory location
            const std::size_t new_capacity = size();
            T* new_data = std::allocator_traits<A>::allocate(allocator_, new_capacity);
            move_range(data_, data_ + size_, new_data);

            // Clear old memory
            destroy_range(data_, data_ + size_);
            std::allocator_traits<A>::deallocate(allocator_, data_, capacity_);

            data_ = new_data;
            capacity_ = new_capacity;
        }
    }

    // Iterators
    template <typename T, typename A>
    constexpr vector<T, A>::iterator vector<T, A>::begin() noexcept
    {
        return iterator(data());
    }

    template <typename T, typename A>
    constexpr vector<T, A>::iterator vector<T, A>::end() noexcept
    {
        return iterator(data() + size());
    }

    template <typename T, typename A>
    constexpr vector<T, A>::const_iterator vector<T, A>::begin() const noexcept
    {
        return const_iterator(data_); // data() will not work because it returns const T*
    }

    template <typename T, typename A>
    constexpr vector<T, A>::const_iterator vector<T, A>::end() const noexcept
    {
        return const_iterator(data_ + size_);
    }

    template <typename T, typename A>
    constexpr vector<T, A>::const_iterator vector<T, A>::cbegin() const noexcept
    {
        return begin();
    }

    template <typename T, typename A>
    constexpr vector<T, A>::const_iterator vector<T, A>::cend() const noexcept
    {
        return end();
    }

    // Reverse iterators
    template <typename T, typename A>
    constexpr vector<T, A>::reverse_iterator vector<T, A>::rbegin() noexcept
    {
        return reverse_iterator(end());
    }

    template <typename T, typename A>
    constexpr vector<T, A>::reverse_iterator vector<T, A>::rend() noexcept
    {
        return reverse_iterator(begin());
    }

    template <typename T, typename A>
    constexpr vector<T, A>::const_reverse_iterator vector<T, A>::rbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }

    template <typename T, typename A>
    constexpr vector<T, A>::const_reverse_iterator vector<T, A>::rend() const noexcept
    {
        return const_reverse_iterator(begin());
    }

    template <typename T, typename A>
    constexpr vector<T, A>::const_reverse_iterator vector<T, A>::crbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }

    template <typename T, typename A>
    constexpr vector<T, A>::const_reverse_iterator vector<T, A>::crend() const noexcept
    {
        return const_reverse_iterator(begin());
    }

    // Private member functions
    template <typename T, typename A>
    bool vector<T, A>::is_memory_filled() const
    {
        return size() == capacity();
    }

    template <typename T, typename A>
    vector<T, A>::size_type vector<T, A>::calculate_new_capacity() const
    {
        constexpr std::size_t growth_factor = 2;

        if (capacity_ == 0) {
            return 1;
        } else {
            return capacity_ * growth_factor;
        }
    }

    template <typename T, typename A>
    void vector<T, A>::grow()
    {
        std::size_t new_capacity = calculate_new_capacity();

        // Move data to new memory location
        T* new_data = std::allocator_traits<A>::allocate(allocator_, new_capacity);
        move_range(data_, data_ + size_, new_data);

        // Clear old memory
        destroy_range(data_, data_ + size_);
        std::allocator_traits<A>::deallocate(allocator_, data_, capacity_);

        data_ = new_data;
        capacity_ = new_capacity;
    }

    template <typename T, typename A>
    constexpr void vector<T, A>::destroy_range(pointer begin, pointer end)
    {
        for (; begin != end; ++begin) {
            std::allocator_traits<A>::destroy(allocator_, begin);
        }
    }

    template <typename T, typename A>
    constexpr void vector<T, A>::default_construct_range(pointer begin, pointer end)
    {
        for (; begin != end; ++begin) {
            std::allocator_traits<A>::construct(allocator_, begin);
        }
    }

    template <typename T, typename A>
    constexpr void vector<T, A>::copy_construct_range(pointer begin, pointer end, const T& value)
    {
        for (; begin != end; ++begin) {
            std::allocator_traits<A>::construct(allocator_, begin, value);
        }
    }

    template <typename T, typename A>
    template <typename InputIt, typename OutputIt>
    constexpr void vector<T, A>::copy_range(InputIt source_begin, InputIt source_end, OutputIt destination_begin)
    {
        for (; source_begin != source_end; ++source_begin, ++destination_begin) {
            std::allocator_traits<A>::construct(allocator_, std::to_address(destination_begin), *source_begin);
        }
    }

    template <typename T, typename A>
    constexpr void vector<T, A>::move_range(pointer source_begin, pointer source_end, pointer destination_begin)
    {
        for (; source_begin != source_end; ++source_begin, ++destination_begin) {
            std::allocator_traits<A>::construct(allocator_, destination_begin, std::move(*source_begin));
        }
    }

    template <typename T, typename A>
    constexpr void vector<T, A>::copy_range_backwards(pointer source_begin, pointer source_end, pointer destination_end)
    {
        while (source_end != source_begin) {
            --source_end;
            --destination_end;

            std::allocator_traits<A>::construct(allocator_, destination_end, *source_end);
        }
    }

    template <typename T, typename A>
    constexpr void vector<T, A>::move_range_backwards(pointer source_begin, pointer source_end, pointer destination_end)
    {
        while (source_end != source_begin) {
            --source_end;
            --destination_end;

            std::allocator_traits<A>::construct(allocator_, destination_end, std::move(*source_end));
        }
    }

    template <typename T, typename A>
    constexpr void vector<T, A>::copy_assign_range(pointer begin, pointer end, const T& value)
    {
        for (; begin != end; ++begin) {
            *begin = value;
        }
    }

    template <typename T, typename A>
    template <typename InputIt, typename OutputIt>
    constexpr void vector<T, A>::copy_assign_range_to_range(InputIt source_begin, InputIt source_end, OutputIt destination_begin)
    {
        for (; source_begin != source_end; ++source_begin, ++destination_begin) {
            *destination_begin = *source_begin;
        }
    }

    template <typename T, typename A>
    constexpr void vector<T, A>::move_assign_range_backwards(pointer source_begin, pointer source_end, pointer destination_end)
    {
        while (source_end != source_begin) {
            --source_end;
            --destination_end;

            *destination_end = std::move(*source_end);
        }
    }

    template <typename T, typename A>
    constexpr void vector<T, A>::move_assign_range(pointer source_begin, pointer source_end, pointer destination_begin)
    {
        for (; source_begin != source_end; ++source_begin, ++destination_begin) {
            *destination_begin = std::move(*source_begin);
        }
    }
}

#endif /* VECTOR_HPP */
