#ifndef DEQUE_HPP
#define DEQUE_HPP

#include "deque_data.hpp"
#include "deque_iterator.hpp"
#include "iterator.hpp"
#include "algorithm.hpp"

#include <memory>
#include <cassert>
#include <stdexcept>
#include <deque>

namespace my
{
    template <typename T>
    constexpr T ceil_division(T a, T b)
    {
        return (a + b - 1) / b;
    }

    template <typename T, typename Allocator = std::allocator<T>>
    class deque
    {
    public:
        // Member types
        using value_type = T;
        using allocator_type = Allocator;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = std::allocator_traits<Allocator>::pointer;
        using const_pointer = std::allocator_traits<Allocator>::const_pointer;
    
        // Implementation specific type aliases
        using deque_data_type = deque_data<value_type, size_type>;
        using block_type = typename deque_data_type::block_type; // Chunk of memory with size = block_size

        // Implementation specific constants
        constexpr static size_type block_size = deque_data_type::block_size; // Size as number of elements not bytes
        
        // Iterator types
        using iterator = deque_iterator<deque>;
        using const_iterator = my::const_iterator<iterator>;
        using reverse_iterator = my::reverse_iterator<iterator>;
        using const_reverse_iterator = my::const_iterator<reverse_iterator>;

        // Constructors
        constexpr deque(); // Standard says nothing about nothrow so we assume it can throw https://stackoverflow.com/questions/15191741/does-default-constructor-of-deque-throw
        constexpr explicit deque(const Allocator& allocator);
        constexpr deque(size_type count, const_reference value, const Allocator& allocator = Allocator());
        constexpr explicit deque(size_type count, const Allocator& allocator = Allocator());
        template <std::input_iterator InputIt>
        constexpr deque(InputIt first, InputIt last, const Allocator& allocator = Allocator());
        constexpr deque(std::initializer_list<T> init_list, const Allocator& allocator = Allocator());

        // constexpr deque(const deque& other, const Allocator& allocator);
        // constexpr deque(deque&& other, const Allocator& allocator);

        // Rule of 5
        constexpr deque(const deque& other);
        constexpr deque& operator=(const deque& other);

        constexpr deque(deque&& other);
        constexpr deque& operator=(deque&& other);
        
        constexpr ~deque();

        // Other
        constexpr allocator_type get_allocator() const noexcept;

        // Element access
        constexpr reference operator[](size_type index);
        constexpr const_reference operator[](size_type index) const;

        constexpr reference at(size_type index);
        constexpr const_reference at(size_type index) const;

        constexpr reference front();
        constexpr const_reference front() const;

        constexpr reference back();
        constexpr const_reference back() const;

        // Capacity
        constexpr [[nodiscard]] bool empty() const noexcept;
        constexpr size_type size() const noexcept;

        // Modifiers
        constexpr void clear() noexcept;

        constexpr void push_back(const T& value);
        constexpr void push_back(T&& value);
        template <typename ... Args>
        constexpr reference emplace_back(Args&& ... args);

        constexpr void push_front(const T& value);
        constexpr void push_front(T&& value);
        template <typename ... Args>
        constexpr reference emplace_front(Args&& ... args);

        constexpr void pop_back();
        constexpr void pop_front();

        constexpr void resize(size_type new_size);
        constexpr void resize(size_type new_size, const value_type& value);

        constexpr void swap(deque& other) noexcept;

        constexpr iterator insert(const_iterator pos, const T& value);
        constexpr iterator insert(const_iterator pos, T&& value);
        constexpr iterator insert(const_iterator pos, size_type count, const T& value);
        template <std::input_iterator InputIt>
        constexpr iterator insert(const_iterator pos, InputIt first, InputIt last);
        constexpr iterator insert(const_iterator pos, std::initializer_list<T> init_list);

        template< class... Args >
        constexpr iterator emplace(const_iterator pos, Args&&... args);

        constexpr iterator erase(const_iterator pos);
        constexpr iterator erase(const_iterator first, const_iterator last);

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
        using element_allocator_type = allocator_type;
        using block_allocator_type = typename std::allocator_traits<allocator_type>::template rebind_alloc<block_type>;

        // Implementation specific member functions
        constexpr void grow_capacity();
        constexpr void grow_capacity_to_fit(size_type new_size);

        constexpr size_type capacity() const;
        constexpr bool is_memory_filled() const;

        constexpr size_type calculate_end_index() const;

        // Functions for converting indices from range [0, capacity) to block index and offset and back
        constexpr size_type calculate_block_index(size_type element_index) const;
        constexpr size_type calculate_block_offset(size_type element_index) const;
        constexpr size_type calculate_element_index(size_type block_index, size_type block_offset) const;

        // Basically - and + modulo capacity
        constexpr size_type calculate_previous_index(size_type current_index, size_type offset = 1) const;
        constexpr size_type calculate_next_index(size_type current_index, size_type offset = 1) const;

        constexpr size_type calculate_free_back_elements_count() const; // TODO: Maybe replace this with capacity_back and remove?

        constexpr bool adding_back_element_would_break_invariant() const;
        constexpr bool adding_front_element_would_break_invariant() const;

        constexpr void destroy_all_elements();
        constexpr void deallocate_all_blocks();
        constexpr void deallocate_blocks_array();

        constexpr void destroy_range(size_type range_begin, size_type range_size);
        constexpr void default_construct_range(size_type range_begin, size_type range_size);
        constexpr void copy_construct_range_values(size_type range_begin, size_type range_size, const value_type& value);
        template <std::input_iterator InputIt>
        constexpr void copy_construct_range_values(size_type range_begin, size_type range_size, InputIt first, InputIt last);

        constexpr void move_assign_range(size_type source_begin, size_type source_end, size_type destination_begin);
        constexpr void move_assign_range_backwards(size_type source_begin, size_type source_end, size_type destination_end);

        constexpr void allocate_blocks(block_type* begin_block, block_type* end_block);
        constexpr void allocate_blocks_unsafe(block_type* begin_block, block_type* end_block);
        constexpr void allocate_blocks_if_not_allocated(block_type* begin_block, block_type* end_block);

        constexpr bool all_blocks_are_allocated() const;

        constexpr size_type next_block_index(size_type block_index) const;
        constexpr size_type previous_block_index(size_type block_index) const;

        // This would be capacity if all blocks were allocated
        constexpr size_type potential_capacity_back() const;
        constexpr size_type potential_capacity_front() const;

        constexpr void reserve_back(size_type n);
        constexpr void reserve_front(size_type n);

        constexpr void move_construct_range(size_type source_begin, size_type source_end, size_type destination_begin);
        constexpr void copy_assign_range_values(size_type destination_begin, size_type destination_end, const value_type& value);
        constexpr void copy_assign_range(size_type source_begin, size_type source_end, size_type destination_begin);
        template <std::input_iterator InputIt>
        constexpr void copy_assign_range(InputIt source_begin, InputIt source_end, size_type destination_begin);

        // Member variables
        element_allocator_type element_allocator { };
        block_allocator_type block_allocator { }; // Name is a bit confusing, because it allocated arrays of pointers to blocks and not actual blocks
        
        deque_data_type data;
    };

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::deque()
    {

    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::deque(const Allocator& allocator) :
        element_allocator(allocator),
        block_allocator(allocator)
    {

    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::deque(size_type count, const_reference value, const Allocator& allocator) :
        deque(allocator)
    {
        resize(count, value);
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::deque(size_type count, const Allocator& allocator) :
        deque(allocator)
    {
        resize(count);
    }

    template <typename T, typename Allocator>
    template <std::input_iterator InputIt>
    constexpr deque<T, Allocator>::deque(InputIt first, InputIt last, const Allocator& allocator) :
        deque(allocator)
    {
        for (; first != last; ++first) {
            emplace_back(*first);
        }
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::deque(std::initializer_list<T> init_list, const Allocator& allocator) :
        deque(std::begin(init_list), std::end(init_list), allocator)
    {

    }


    // Rule of 5
    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::deque(const deque& other) :
        element_allocator(std::allocator_traits<element_allocator_type>::select_on_container_copy_construction(other.get_allocator())),
        block_allocator(std::allocator_traits<element_allocator_type>::select_on_container_copy_construction(other.get_allocator())),
        data()
    {
        data.elements_count = other.data.elements_count;
        data.blocks_count = (data.elements_count + block_size - 1) / block_size; // Ceil division
        
        if (data.blocks_count > 0) {
            // Allocate just enough blocks to store all elements
            data.blocks = std::allocator_traits<block_allocator_type>::allocate(block_allocator, data.blocks_count);
            for (size_type i = 0; i < data.blocks_count; ++i) {
                data.blocks[i] = std::allocator_traits<element_allocator_type>::allocate(element_allocator, block_size);
            }

            // Copy all elements
            for (size_type i = 0; i < data.elements_count; ++i) {
                const auto block_index = calculate_block_index(i);
                const auto block_offset = calculate_block_offset(i);

                std::allocator_traits<element_allocator_type>::construct(
                    element_allocator,
                    data.blocks[block_index] + block_offset,
                    other[i]
                );
            }
        }
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>& deque<T, Allocator>::operator=(const deque& other)
    {
        // Can be optimized
        deque copy(other);
        copy.swap(*this);
        return *this;
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::deque(deque&& other) :
        element_allocator(std::move(other.element_allocator)),
        block_allocator(std::move(other.block_allocator)),
        data(other.data)
    {
        other.data = {};
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>& deque<T, Allocator>::operator=(deque&& other)
    {
        this->swap(other);
        return *this;
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::~deque()
    {
        destroy_all_elements();
        deallocate_all_blocks();
        deallocate_blocks_array();
    }


    // Other
    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::allocator_type deque<T, Allocator>::get_allocator() const noexcept
    {
        return element_allocator;
    }


    // Element access
    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::reference deque<T, Allocator>::operator[](size_type index)
    {
        const auto physical_index = calculate_next_index(data.begin_index, index);
        const auto block_index = calculate_block_index(physical_index);
        const auto block_offset = calculate_block_offset(physical_index);
        return data.blocks[block_index][block_offset];
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::const_reference deque<T, Allocator>::operator[](size_type index) const
    {
        const auto physical_index = calculate_next_index(data.begin_index, index);
        const auto block_index = calculate_block_index(physical_index);
        const auto block_offset = calculate_block_offset(physical_index);
        return data.blocks[block_index][block_offset];
    }


    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::reference deque<T, Allocator>::at(size_type index)
    {
        if (index >= data.elements_count) {
            throw std::out_of_range("Invalid element index");
        }

        return (*this)[index];
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::const_reference deque<T, Allocator>::at(size_type index) const
    {
        if (index >= data.elements_count) {
            throw std::out_of_range("Invalid element index");
        }

        return (*this)[index];
    }


    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::reference deque<T, Allocator>::front()
    {
        const auto block_index = calculate_block_index(data.begin_index);
        const auto block_offset = calculate_block_offset(data.begin_index);

        return data.blocks[block_index][block_offset];
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::const_reference deque<T, Allocator>::front() const
    {
        const auto block_index = calculate_block_index(data.begin_index);
        const auto block_offset = calculate_block_offset(data.begin_index);
        
        return data.blocks[block_index][block_offset];
    }


    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::reference deque<T, Allocator>::back()
    {
        const auto last_element_index = calculate_previous_index(calculate_end_index());
        auto block_index = calculate_block_index(last_element_index);
        auto block_offset = calculate_block_offset(last_element_index);

        return data.blocks[block_index][block_offset];
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::const_reference deque<T, Allocator>::back() const
    {
        const auto last_element_index = calculate_previous_index(calculate_end_index());
        auto block_index = calculate_block_index(last_element_index);
        auto block_offset = calculate_block_offset(last_element_index);

        return data.blocks[block_index][block_offset];
    }


    // Capacity
    template <typename T, typename Allocator>
    constexpr [[nodiscard]] bool deque<T, Allocator>::empty() const noexcept
    {
        return size() == 0;
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::size_type deque<T, Allocator>::size() const noexcept
    {
        return data.elements_count;
    }


    // Modifiers
    template <typename T, typename Allocator>
    constexpr void deque<T, Allocator>::clear() noexcept
    {
        destroy_all_elements();
        // Begin index remains the same to utilize memory of already allocated blocks
        data.elements_count = 0;
    }


    template <typename T, typename Allocator>
    constexpr void deque<T, Allocator>::push_back(const T& value)
    {
        emplace_back(value);
    }

    template <typename T, typename Allocator>
    constexpr void deque<T, Allocator>::push_back(T&& value)
    {
        emplace_back(std::move(value));
    }

    template <typename T, typename Allocator>
    template <typename ... Args>
    constexpr deque<T, Allocator>::reference deque<T, Allocator>::emplace_back(Args&& ... args)
    {
        if (is_memory_filled() || adding_back_element_would_break_invariant()) {
            grow_capacity();
        }
        
        auto end_index = calculate_end_index();
        auto end_block = calculate_block_index(end_index);
        auto end_offset = calculate_block_offset(end_index);

        // Allocate if we step into unallocated block
        // Should not overwrite old blocks
        if (data.blocks[end_index] == nullptr) {
            data.blocks[end_index] = std::allocator_traits<element_allocator_type>::allocate(element_allocator, block_size);
        }
        std::allocator_traits<element_allocator_type>::construct(
            element_allocator,
            data.blocks[end_block] + end_offset,
            std::forward<Args>(args) ...
        );

        ++data.elements_count;

        return data.blocks[end_block][end_offset];
    }


    template <typename T, typename Allocator>
    constexpr void deque<T, Allocator>::push_front(const T& value)
    {
        emplace_front(value);
    }

    template <typename T, typename Allocator>
    constexpr void deque<T, Allocator>::push_front(T&& value)
    {
        emplace_front(std::move(value));
    }

    template <typename T, typename Allocator>
    template <typename ... Args>
    constexpr deque<T, Allocator>::reference deque<T, Allocator>::emplace_front(Args&& ... args)
    {
        if (is_memory_filled() || adding_front_element_would_break_invariant()) {
            grow_capacity();
        }
        
        auto new_begin_index = calculate_previous_index(data.begin_index);
        auto new_begin_block = calculate_block_index(new_begin_index);
        auto new_begin_offset = calculate_block_offset(new_begin_index);

        // Allocate if we step into unallocated block
        // Should not overwrite old blocks
        if (data.blocks[new_begin_block] == nullptr) {
            data.blocks[new_begin_block] = std::allocator_traits<element_allocator_type>::allocate(element_allocator, block_size);
        }
        std::allocator_traits<element_allocator_type>::construct(
            element_allocator,
            data.blocks[new_begin_block] + new_begin_offset,
            std::forward<Args>(args) ...
        );

        data.begin_index = new_begin_index;
        ++data.elements_count;

        return data.blocks[new_begin_block][new_begin_offset];
    }


    template <typename T, typename Allocator>
    constexpr void deque<T, Allocator>::pop_back()
    {
        assert((data.elements_count > 0) && "Trying to remove last element of empty deque is undefined behavior");

        const auto last_element_index = calculate_previous_index(calculate_end_index());
        const auto last_element_block = calculate_block_index(last_element_index);
        const auto last_element_offset = calculate_block_offset(last_element_index);

        std::allocator_traits<element_allocator_type>::destroy(
            element_allocator,
            data.blocks[last_element_block] + last_element_offset
        );

        --data.elements_count;
    }

    template <typename T, typename Allocator>
    constexpr void deque<T, Allocator>::pop_front()
    {
        assert((data.elements_count > 0) && "Trying to remove first element of empty deque is undefined behavior");

        auto begin_block = calculate_block_index(data.begin_index);
        auto begin_offset = calculate_block_offset(data.begin_index);

        std::allocator_traits<element_allocator_type>::destroy(
            element_allocator,
            data.blocks[begin_block] + begin_offset
        );

        data.begin_index = calculate_next_index(data.begin_index);
        --data.elements_count;
    }

    template <typename T, typename Allocator>
    constexpr void deque<T, Allocator>::resize(size_type new_size)
    {
        if (new_size <= size()) {
            const auto number_of_elements_to_destroy = size() - new_size;
            destroy_range(calculate_next_index(data.begin_index, new_size), number_of_elements_to_destroy);
        } else {
            const auto number_of_new_elements = new_size - size();
            grow_capacity_to_fit(new_size);
            default_construct_range(calculate_end_index(), number_of_new_elements);
        }

        data.elements_count = new_size;
    }

    template <typename T, typename Allocator>
    constexpr void deque<T, Allocator>::resize(size_type new_size, const value_type& value)
    {
        if (new_size <= size()) {
            const auto number_of_elements_to_destroy = size() - new_size;
            destroy_range(calculate_next_index(data.begin_index, new_size), number_of_elements_to_destroy);
        } else {
            const auto number_of_new_elements = new_size - size();
            grow_capacity_to_fit(new_size);
            copy_construct_range_values(calculate_end_index(), number_of_new_elements, value);
        }

        data.elements_count = new_size;
    }

    template <typename T, typename Allocator>
    constexpr void deque<T, Allocator>::swap(deque& other) noexcept
    {
        using std::swap;

        if constexpr (std::allocator_traits<allocator_type>::propagate_on_container_swap::value) {
            std::swap(block_allocator, other.block_allocator);
            std::swap(element_allocator, other.element_allocator);
        }

        std::swap(data.blocks, other.data.blocks);
        std::swap(data.blocks_count, other.data.blocks_count);
        std::swap(data.begin_index, other.data.begin_index);
        std::swap(data.elements_count, other.data.elements_count);
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::iterator deque<T, Allocator>::insert(const_iterator pos, const T& value)
    {
        return emplace(pos, value);
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::iterator deque<T, Allocator>::insert(const_iterator pos, T&& value)
    {
        return emplace(pos, std::move(value));
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::iterator deque<T, Allocator>::insert(const_iterator pos, size_type count, const T& value)
    {
        if (count == 0) {
            return begin() + (pos - cbegin());
        }

        if (pos == cbegin()) {
            reserve_front(count);
            copy_construct_range_values(calculate_previous_index(data.begin_index, count), count, value);
            data.elements_count += count;
            data.begin_index = calculate_previous_index(data.begin_index, count);
            
            return begin();
        }
        
        if (pos == cend()) {
            reserve_back(count);
            copy_construct_range_values(calculate_end_index(), count, value);
            data.elements_count += count;

            return end();
        }

        bool is_pos_closer_to_begin = pos - cbegin() < cend() - pos;
        if (is_pos_closer_to_begin) {
            reserve_front(count);
            const auto elements_to_move = pos - cbegin();

            if (count < elements_to_move) {
                // First count elements are moved into uninitialized memory
                move_construct_range(
                    data.begin_index,
                    calculate_next_index(data.begin_index, count),
                    calculate_previous_index(data.begin_index, count)
                );

                // Other elements are moved to begin
                move_assign_range(
                    calculate_next_index(data.begin_index, count),
                    calculate_next_index(data.begin_index, elements_to_move),
                    data.begin_index
                );

                // The gap is filled with copies of value
                copy_assign_range_values(
                    calculate_next_index(data.begin_index, elements_to_move - count),
                    calculate_next_index(data.begin_index, elements_to_move),
                    value
                );
            }

            if (count == elements_to_move) {
                // Trivial case
                // All elements before pos are moved to uninitialized memory
                move_construct_range(
                    data.begin_index, 
                    calculate_next_index(data.begin_index, elements_to_move),
                    calculate_previous_index(data.begin_index, elements_to_move)
                );

                // Assign copies of value to moved-from objects
                copy_assign_range_values(
                    data.begin_index, 
                    calculate_next_index(data.begin_index, elements_to_move),
                    value
                );
            }

            if (count > elements_to_move) {
                // Move first (elements_to_move) elements to uninitialized memory
                move_construct_range(
                    data.begin_index,
                    calculate_next_index(data.begin_index, elements_to_move),
                    calculate_previous_index(data.begin_index, count)
                );

                // Construct (count - elements_to_move) copies of value in uninitialized memory
                copy_construct_range_values(
                    calculate_previous_index(data.begin_index, count - elements_to_move),
                    count - elements_to_move, // This is not end index, but size of range
                    value
                );

                // Fill moved-from elements with copies of value 
                copy_assign_range_values(
                    data.begin_index, 
                    calculate_next_index(data.begin_index, elements_to_move),
                    value
                );
            }

            data.begin_index = calculate_previous_index(data.begin_index, count);
            data.elements_count += count;
            return begin() + elements_to_move;
        } else {
            reserve_back(count);
            const auto elements_to_move = cend() - pos;
            const auto end_index = calculate_end_index();

            if (count < elements_to_move) {
                // Move last (count) elements to uninitialized memory
                move_construct_range(
                    calculate_previous_index(end_index, count),
                    end_index,
                    end_index
                );

                // Move other (elements_to_move - count) elements to the end
                move_assign_range(
                    calculate_previous_index(end_index, elements_to_move),
                    calculate_previous_index(end_index, count),
                    calculate_previous_index(end_index, elements_to_move - count)
                );

                // Fill the gap with the copies of value
                copy_assign_range_values(
                    calculate_previous_index(end_index, elements_to_move),
                    calculate_previous_index(end_index, elements_to_move - count),
                    value
                );
            }

            if (count == elements_to_move) {
                // Move all elements after pos (including pos) to uninitialized memory
                move_construct_range(
                    calculate_previous_index(end_index, count),
                    end_index,
                    end_index
                );

                // Copy assign value to all moved-from objects
                copy_assign_range_values(
                    calculate_previous_index(end_index, count), 
                    end_index,
                    value
                );
            }

            if (count > elements_to_move) {
                // Move all elements after pos (including pos) (count) places forward to make space for new values
                move_construct_range(
                    calculate_previous_index(end_index, elements_to_move),
                    end_index,
                    calculate_next_index(end_index, count - elements_to_move)
                );

                // Construct (count - elements_to_move) copies of value in uninitialized memory
                copy_construct_range_values(
                    end_index,
                    count - elements_to_move, // Number of elements
                    value
                );

                // Assign copies of value to moved-from elements
                copy_assign_range_values(
                    calculate_previous_index(end_index, elements_to_move),
                    end_index,
                    value
                );
            }

            data.elements_count += count;
            return end() - (count + elements_to_move);
        }
    }

    template <typename T, typename Allocator>
    template <std::input_iterator InputIt>
    constexpr deque<T, Allocator>::iterator deque<T, Allocator>::insert(const_iterator pos, InputIt first, InputIt last)
    {
        const auto count = std::distance(first, last);
        if (count == 0) {
            return begin() + (pos - cbegin());
        }

        if (pos == cbegin()) {
            reserve_front(count);
            copy_construct_range_values(calculate_previous_index(data.begin_index, count), count, first, last);
            data.elements_count += count;
            data.begin_index = calculate_previous_index(data.begin_index, count);
            
            return begin();
        }
        
        if (pos == cend()) {
            reserve_back(count);
            copy_construct_range_values(calculate_end_index(), count, first, last);
            data.elements_count += count;

            return end();
        }

        bool is_pos_closer_to_begin = pos - cbegin() < cend() - pos;
        if (is_pos_closer_to_begin) {
            reserve_front(count);
            const auto elements_to_move = pos - cbegin();

            if (count < elements_to_move) {
                // First count elements are moved into uninitialized memory
                move_construct_range(
                    data.begin_index,
                    calculate_next_index(data.begin_index, count),
                    calculate_previous_index(data.begin_index, count)
                );

                // Other elements are moved to begin
                move_assign_range(
                    calculate_next_index(data.begin_index, count),
                    calculate_next_index(data.begin_index, elements_to_move),
                    data.begin_index
                );

                // Fill the gap with elements from iterator range
                copy_assign_range(
                    first,
                    last,
                    calculate_next_index(data.begin_index, elements_to_move - count)
                );
            }

            if (count == elements_to_move) {
                // Trivial case
                // All elements before pos are moved to uninitialized memory
                move_construct_range(
                    data.begin_index, 
                    calculate_next_index(data.begin_index, elements_to_move),
                    calculate_previous_index(data.begin_index, elements_to_move)
                );

                // Assign copies of elements from iterator range to moved-from objects
                copy_assign_range(
                    first,
                    last,
                    data.begin_index
                );
            }

            if (count > elements_to_move) {
                // Move first (elements_to_move) elements to uninitialized memory
                move_construct_range(
                    data.begin_index,
                    calculate_next_index(data.begin_index, elements_to_move),
                    calculate_previous_index(data.begin_index, count)
                );

                // Construct (count - elements_to_move) elements from iterator range in uninitialized memory
                copy_construct_range_values(
                    calculate_previous_index(data.begin_index, count - elements_to_move),
                    count - elements_to_move,
                    first,
                    last
                );

                // Fill moved-from elements with copies of elements from iterator range
                copy_assign_range(
                    first + (count - elements_to_move),
                    last,
                    data.begin_index
                );
            }

            data.begin_index = calculate_previous_index(data.begin_index, count);
            data.elements_count += count;
            return begin() + elements_to_move;
        } else {
            reserve_back(count);
            const auto elements_to_move = cend() - pos;
            const auto end_index = calculate_end_index();

            if (count < elements_to_move) {
                // Move last (count) elements to uninitialized memory
                move_construct_range(
                    calculate_previous_index(end_index, count),
                    end_index,
                    end_index
                );

                // Move other (elements_to_move - count) elements to the end
                move_assign_range(
                    calculate_previous_index(end_index, elements_to_move),
                    calculate_previous_index(end_index, count),
                    calculate_previous_index(end_index, elements_to_move - count)
                );

                // Copy values from iterator range
                copy_assign_range(
                    first,
                    last,
                    calculate_previous_index(end_index, elements_to_move)
                );
            }

            if (count == elements_to_move) {
                // Move all elements after pos (including pos) to uninitialized memory
                move_construct_range(
                    calculate_previous_index(end_index, count),
                    end_index,
                    end_index
                );

                // Copy values from iterator range
                copy_assign_range(
                    first,
                    last,
                    calculate_previous_index(end_index, count)
                );
            }

            if (count > elements_to_move) {
                // Move all elements after pos (including pos) (count) places forward to make space for new values
                move_construct_range(
                    calculate_previous_index(end_index, elements_to_move),
                    end_index,
                    calculate_next_index(end_index, count - elements_to_move)
                );

                // Construct (count - elements_to_move) values from iterator range in uninitialized memory
                copy_construct_range_values(
                    end_index,
                    count - elements_to_move, // Number of elements
                    first + elements_to_move,
                    last
                );

                // Assign copies of value to moved-from elements
                copy_assign_range(
                    first,
                    first + elements_to_move,
                    calculate_previous_index(end_index, elements_to_move)
                );
            }

            data.elements_count += count;
            return end() - (count + elements_to_move);
        }
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::iterator deque<T, Allocator>::insert(const_iterator pos, std::initializer_list<T> init_list)
    {
        return insert(pos, std::begin(init_list), std::end(init_list));
    }

    template <typename T, typename Allocator>
    template< class... Args >
    constexpr deque<T, Allocator>::iterator deque<T, Allocator>::emplace(const_iterator pos, Args&&... args)
    {
        if (pos == cbegin()) {
            emplace_front(std::forward<Args>(args) ...);
            return begin();
        } else if (pos == cend()) {
            emplace_back(std::forward<Args>(args) ...);
            return end();
        } else {
            if (is_memory_filled()) {
                grow_capacity();
            }

            if (pos - begin() < end() - pos) {
                // Move elements before pos
                if (adding_front_element_would_break_invariant()) {
                    grow_capacity();
                }

                const auto new_begin = calculate_previous_index(data.begin_index);
                const auto new_begin_block = calculate_block_index(new_begin);
                const auto new_begin_offset = calculate_block_offset(new_begin);
                
                std::allocator_traits<element_allocator_type>::construct(
                    element_allocator,
                    data.blocks[new_begin_block] + new_begin_offset,
                    std::move(front())
                );
                move_assign_range(
                    data.begin_index,
                    calculate_next_index(data.begin_index, pos - begin()),
                    calculate_previous_index(data.begin_index)
                );

                const auto insert_position = calculate_previous_index(calculate_next_index(data.begin_index, pos - begin()));
                const auto insert_position_block = calculate_block_index(insert_position);
                const auto insert_position_offset = calculate_block_offset(insert_position);
                // Destroy moved-from object
                std::allocator_traits<element_allocator_type>::destroy(
                    element_allocator,
                    data.blocks[insert_position_block] + insert_position_offset
                );
                // Create new one
                std::allocator_traits<element_allocator_type>::construct(
                    element_allocator,
                    data.blocks[insert_position_block] + insert_position_offset,
                    std::forward<Args>(args) ...
                );
            
                data.begin_index = new_begin;
                ++data.elements_count;
            } else {
                // Move elements after pos (including pos)
                if (adding_back_element_would_break_invariant()) {
                    grow_capacity();
                }

                const auto new_element_position = calculate_end_index();
                const auto new_element_block = calculate_block_index(new_element_position);
                const auto new_element_offset = calculate_block_offset(new_element_position);
                
                std::allocator_traits<element_allocator_type>::construct(
                    element_allocator,
                    data.blocks[new_element_block] + new_element_offset,
                    std::move(back())
                );
                move_assign_range_backwards(
                    calculate_next_index(data.begin_index, pos - begin()),
                    calculate_previous_index(new_element_position),
                    new_element_position
                );

                const auto insert_position = calculate_next_index(data.begin_index, pos - begin());
                const auto insert_position_block = calculate_block_index(insert_position);
                const auto insert_position_offset = calculate_block_offset(insert_position);
                // Destroy moved-from object
                std::allocator_traits<element_allocator_type>::destroy(
                    element_allocator,
                    data.blocks[insert_position_block] + insert_position_offset
                );
                // Create new one
                std::allocator_traits<element_allocator_type>::construct(
                    element_allocator,
                    data.blocks[insert_position_block] + insert_position_offset,
                    std::forward<Args>(args) ...
                );

                // Data is moved forward so begin_index does not change
                ++data.elements_count;
            }

            // In both cases index of newly inserted element is the same as in pos
            // Pos could be reused but I don't want to depend on iterator implementation
            return iterator(&data, pos - begin());
        }
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::iterator deque<T, Allocator>::erase(const_iterator first, const_iterator last)
    {
        if (first == last) {
            return last;
        }

        const auto count = last - first;

        if (first == cbegin()) {
            destroy_range(data.begin_index, count);
            data.begin_index = calculate_next_index(data.begin_index, count);
            data.elements_count -= count;
            return begin();
        }

        if (last == cend()) {
            destroy_range(calculate_next_index(data.begin_index, first - cbegin()), count);
            data.elements_count -= count;
            return end();
        }

        // Move minimal amount of elements
        const auto elements_before = first - cbegin();
        const auto elements_after = cend() - last;
        if (elements_before < elements_after) {
            move_assign_range_backwards(
                data.begin_index,
                calculate_next_index(data.begin_index, first - cbegin()),
                calculate_next_index(data.begin_index, last - cbegin())
            );

            destroy_range(data.begin_index, count);
            data.begin_index = calculate_next_index(data.begin_index, count);
            data.elements_count -= count;
        } else {
            move_assign_range(
                calculate_next_index(data.begin_index, last - cbegin()),
                calculate_end_index(),
                calculate_next_index(data.begin_index, first - cbegin())
            );

            destroy_range(calculate_previous_index(calculate_end_index(), count), count);
            data.elements_count -= count; 
        }

        return begin() + elements_before;
    }


    // Iterators
    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::iterator deque<T, Allocator>::begin() noexcept
    {
        return iterator(&data, 0);
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::iterator deque<T, Allocator>::end() noexcept
    {
        return iterator(&data, data.elements_count);
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::const_iterator deque<T, Allocator>::begin() const noexcept
    {
        // I know const_cast is bad, but i don't know if there is a safe way to do this
        // (i don't want to implement const_iterator from scratch after spending so much time on basic_const_iterator)
        return const_iterator(iterator(const_cast<deque_data_type*>(&data), 0));
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::const_iterator deque<T, Allocator>::end() const noexcept
    {
        return const_iterator(iterator(const_cast<deque_data_type*>(&data),  data.elements_count));
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::const_iterator deque<T, Allocator>::cbegin() const noexcept
    {
        return begin();
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::const_iterator deque<T, Allocator>::cend() const noexcept
    {
        return end();
    }


    // Reverse iterators
    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::reverse_iterator deque<T, Allocator>::rbegin() noexcept
    {
        return reverse_iterator(end());
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::reverse_iterator deque<T, Allocator>::rend() noexcept
    {
        return reverse_iterator(begin());
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::const_reverse_iterator deque<T, Allocator>::rbegin() const noexcept
    {
        return const_reverse_iterator(reverse_iterator(const_cast<deque*>(this)->end()));
    }
    
    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::const_reverse_iterator deque<T, Allocator>::rend() const noexcept
    {
        return const_reverse_iterator(reverse_iterator(const_cast<deque*>(this)->begin()));
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::const_reverse_iterator deque<T, Allocator>::crbegin() const noexcept
    {
        return rbegin();
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::const_reverse_iterator deque<T, Allocator>::crend() const noexcept
    {
        return rend();
    }


    // Non-member functions
    template <class T, class Allocator>
    constexpr bool operator==(const deque<T, Allocator>& a, const deque<T, Allocator>& b)
    {
        if (a.size() != b.size()) {
            return false;
        }

        auto i = a.begin();
        auto j = b.begin();
        for (; i != a.end(); ++i, ++j) {
            if (*i != *j) {
                return false;
            }
        }

        return true;
    }

    template <class T, class Allocator>
    constexpr my::synth_three_way_result<T> operator<=>(const deque<T, Allocator>& a, const deque<T, Allocator>& b)
    {
        return my::lexicographical_compare_three_way(
            a.begin(), a.end(),
            b.begin(), b.end(),
            my::synth_three_way { }
        );
    }


    // Implementation details
    template <typename T, typename Allocator>
    constexpr void deque<T, Allocator>::grow_capacity()
    {
        size_type new_blocks_count;
        if (data.blocks_count == 0) {
            new_blocks_count = 2; // Minimum 2 blocks, number is arbitrary
        } else {
            new_blocks_count = data.blocks_count * 2;
        }

        auto new_blocks = std::allocator_traits<block_allocator_type>::allocate(block_allocator, new_blocks_count);
        auto begin_block_index = calculate_block_index(data.begin_index);
        auto begin_block_offset = calculate_block_offset(data.begin_index);
        // We copy blocks before begin block to the start of the new memory
        std::copy(data.blocks, data.blocks + begin_block_index, new_blocks);
        // And all blocks after (including begin block) to the end
        // Backwards because calculating d_first is more complicated than d_last
        std::copy_backward(data.blocks + begin_block_index, data.blocks + data.blocks_count, new_blocks + new_blocks_count);

        auto new_begin_block_index = new_blocks_count - (data.blocks_count - begin_block_index);

        // Don't forget to fill the gap between the two with nullptrs
        std::fill(new_blocks + begin_block_index, new_blocks + new_begin_block_index, nullptr);

        // Clear block pointers array
        std::allocator_traits<block_allocator_type>::deallocate(block_allocator, data.blocks, data.blocks_count);

        data.begin_index = calculate_element_index(new_begin_block_index, begin_block_offset) % (new_blocks_count * block_size);
        data.blocks = new_blocks;
        data.blocks_count = new_blocks_count;
        // elements_count is not changed
    }

    template <typename T, typename Allocator>
    constexpr void deque<T, Allocator>::grow_capacity_to_fit(size_type new_size)
    {
        const auto new_elements_count = new_size - size();
        const auto free_elements_count = calculate_free_back_elements_count();

        if (new_elements_count <= free_elements_count) {
            // No need to iterate all blocks, but i don't feel like calculating which ones are/aren't allocated
            allocate_blocks_if_not_allocated(data.blocks, data.blocks + data.blocks_count);
            return;
        }

        const auto extra_elements_count = new_elements_count - free_elements_count;
        const auto extra_blocks_count = (extra_elements_count + block_size - 1) / block_size;
        const auto new_blocks_count = data.blocks_count + extra_blocks_count;

        auto new_blocks = std::allocator_traits<block_allocator_type>::allocate(block_allocator, new_blocks_count);
        
        auto begin_block_index = calculate_block_index(data.begin_index);
        if (data.blocks == nullptr) {
            allocate_blocks_unsafe(new_blocks, new_blocks + new_blocks_count);
        } else {
            if (data.begin_index < calculate_end_index()) {
                // Deque looks like this:
                //    begin      end
                //      v         v
                // [ | |#|#|#|#|#| ]
                const auto front_empty_blocks_count = begin_block_index;
                const auto back_empty_blocks_count = data.blocks_count - ceil_division(calculate_end_index(), block_size);
                const auto filled_blocks_count = data.blocks_count - (front_empty_blocks_count + back_empty_blocks_count);

                // Copy all blocks
                std::copy(data.blocks, data.blocks + data.blocks_count, new_blocks);
                allocate_blocks_if_not_allocated(new_blocks, new_blocks + front_empty_blocks_count);
                allocate_blocks_if_not_allocated(new_blocks + front_empty_blocks_count + filled_blocks_count, new_blocks + data.blocks_count);
                allocate_blocks(new_blocks + data.blocks_count, new_blocks + new_blocks_count);
            } else {
                // And here we deal with this case:
                //       end    begin
                //        v       v
                // [#|#|#| | | | |#]
                const auto back_blocks_count = data.blocks_count - begin_block_index;
                const auto front_blocks_count = ceil_division(calculate_end_index(), block_size);
                std::copy(data.blocks + begin_block_index, data.blocks + data.blocks_count, new_blocks);
                std::copy(data.blocks, data.blocks + front_blocks_count, new_blocks + back_blocks_count);
                std::copy(
                    data.blocks + front_blocks_count,
                    data.blocks + back_blocks_count,
                    new_blocks + back_blocks_count + front_blocks_count
                ); // Copy free blocks, because they can be empty but allocated and we don't want to leak
                allocate_blocks_if_not_allocated(new_blocks + back_blocks_count + front_blocks_count, new_blocks + data.blocks_count);
                allocate_blocks(new_blocks + data.blocks_count, new_blocks + new_blocks_count);
            }
            deallocate_blocks_array();
        }

        data.begin_index = data.begin_index % block_size;
        data.blocks = new_blocks;
        data.blocks_count = new_blocks_count;

        assert((all_blocks_are_allocated()) && "All blocks must be allocated. It should be safe to construct element in any block after call to this function");
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::size_type deque<T, Allocator>::capacity() const
    {
        return block_size * data.blocks_count;
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::size_type deque<T, Allocator>::calculate_end_index() const
    {
        assert((data.elements_count < capacity()) && "Memory is filled. End index would end up equal to begin index. You should handle this case separately using is_memory_filled");
        return (data.begin_index + data.elements_count) % capacity();
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::size_type deque<T, Allocator>::calculate_block_index(size_type element_index) const
    {
        return element_index / block_size;
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::size_type deque<T, Allocator>::calculate_block_offset(size_type element_index) const
    {
        return element_index % block_size;
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::size_type deque<T, Allocator>::calculate_element_index(size_type block_index, size_type block_offset) const
    {
        return block_index * block_size + block_offset;
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::size_type deque<T, Allocator>::calculate_previous_index(size_type current_index, size_type offset) const
    {
        assert((0 <= offset && offset < capacity()) && "Invalid offset");
        // + capacity to not wrap around
        return (current_index + capacity() - offset) % capacity();
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::size_type deque<T, Allocator>::calculate_next_index(size_type current_index, size_type offset) const
    {
        assert((0 <= offset && offset < capacity()) && "Invalid offset");
        return (current_index + offset) % capacity();
    }

    template <typename T, typename Allocator>
    constexpr bool deque<T, Allocator>::adding_front_element_would_break_invariant() const
    {
        // Blocks like this one are ok:
        //    begin      end
        //      v         v
        // [ | |#|#|#|#|#| ]

        // But with block like this it is not possible to add new blocks without moving some elements
        //       end    begin
        //        v       v
        // [#|#|#| | | | |#]

        auto new_begin_index = calculate_previous_index(data.begin_index);
        auto end_index = calculate_end_index();

        auto new_begin_block = calculate_block_index(new_begin_index);
        auto end_block = calculate_block_index(end_index);

        return (new_begin_index > end_index) && (new_begin_block == end_block);
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::size_type deque<T, Allocator>::calculate_free_back_elements_count() const
    {
        const auto number_of_free_elements_in_first_block = calculate_block_offset(data.begin_index);
        return capacity() - number_of_free_elements_in_first_block; // FIXME: What about all filled elements?
    }

    template <typename T, typename Allocator>
    constexpr bool deque<T, Allocator>::adding_back_element_would_break_invariant() const
    {
        auto end_index = calculate_end_index();
        auto new_end_index = calculate_next_index(end_index);

        auto new_end_block = calculate_block_index(new_end_index);
        auto begin_block = calculate_block_index(data.begin_index);

        return (new_end_index < data.begin_index) && (new_end_block == begin_block);
    }

    template <typename T, typename Allocator>
    constexpr bool deque<T, Allocator>::is_memory_filled() const
    {
        return data.elements_count == capacity();
    }

    template <typename T, typename Allocator>
    constexpr void deque<T, Allocator>::destroy_all_elements()
    {
        auto current_index = data.begin_index;

        for (size_type i = 0; i < data.elements_count; ++i) {
            const auto current_block_index = calculate_block_index(current_index);
            const auto current_block_offset = calculate_block_offset(current_index);

            std::allocator_traits<element_allocator_type>::destroy(
                element_allocator,
                data.blocks[current_block_index] + current_block_offset
            );

            current_index = calculate_next_index(current_index);
        }
    }

    template <typename T, typename Allocator>
    constexpr void deque<T, Allocator>::deallocate_all_blocks()
    {
        for (size_type i = 0; i < data.blocks_count; ++i) {
            if (data.blocks[i] != nullptr) {
                std::allocator_traits<element_allocator_type>::deallocate(element_allocator, data.blocks[i], block_size);
            }
        }
    }

    template <typename T, typename Allocator>
    constexpr void deque<T, Allocator>::deallocate_blocks_array()
    {
        std::allocator_traits<block_allocator_type>::deallocate(block_allocator, data.blocks, data.blocks_count);
    }

    
    template <typename T, typename Allocator>
    constexpr void deque<T, Allocator>::destroy_range(size_type range_begin, size_type range_size)
    {
        for (size_type i = 0; i < range_size; ++i) {
            auto current_block = calculate_block_index(range_begin);
            auto current_offset = calculate_block_offset(range_begin);

            std::allocator_traits<element_allocator_type>::destroy(
                element_allocator,
                data.blocks[current_block] + current_offset
            );

            range_begin = calculate_next_index(range_begin);
        }
    }

    template <typename T, typename Allocator>
    constexpr void deque<T, Allocator>::default_construct_range(size_type range_begin, size_type range_size)
    {
        // range_begin is the same type of index as begin_index
        for (size_type i = 0; i < range_size; ++i) {
            auto current_block = calculate_block_index(range_begin);
            auto current_offset = calculate_block_offset(range_begin);

            std::allocator_traits<element_allocator_type>::construct(
                element_allocator,
                data.blocks[current_block] + current_offset
            );

            range_begin = calculate_next_index(range_begin);
        }
    }

    template <typename T, typename Allocator>
    constexpr void deque<T, Allocator>::copy_construct_range_values(size_type range_begin, size_type range_size, const value_type& value)
    {
        // range_begin is the same type of index as begin_index
        for (size_type i = 0; i < range_size; ++i) {
            auto current_block = calculate_block_index(range_begin);
            auto current_offset = calculate_block_offset(range_begin);

            std::allocator_traits<element_allocator_type>::construct(
                element_allocator,
                data.blocks[current_block] + current_offset,
                value
            );

            range_begin = calculate_next_index(range_begin);
        }
    }

    template <typename T, typename Allocator>
    template <std::input_iterator InputIt>
    constexpr void deque<T, Allocator>::copy_construct_range_values(size_type range_begin, size_type range_size, InputIt first, InputIt last)
    {
        // range_begin is the same type of index as begin_index
        for (size_type i = 0; i < range_size; ++i) {
            auto current_block = calculate_block_index(range_begin);
            auto current_offset = calculate_block_offset(range_begin);

            std::allocator_traits<element_allocator_type>::construct(
                element_allocator,
                data.blocks[current_block] + current_offset,
                *first
            );

            ++first;
            range_begin = calculate_next_index(range_begin);
        }
    }

    template <typename T, typename Allocator>
    constexpr void deque<T, Allocator>::move_assign_range(size_type source_begin, size_type source_end, size_type destination_begin)
    {
        while (source_begin != source_end) {
            auto source_block = calculate_block_index(source_begin);
            auto source_offset = calculate_block_offset(source_begin);

            auto destination_block = calculate_block_index(destination_begin);
            auto destination_offset = calculate_block_offset(destination_begin);

            data.blocks[destination_block][destination_offset] = std::move(data.blocks[source_block][source_offset]);

            source_begin = calculate_next_index(source_begin);
            destination_begin = calculate_next_index(destination_begin);
        }
    }

    template <typename T, typename Allocator>
    constexpr void deque<T, Allocator>::move_assign_range_backwards(size_type source_begin, size_type source_end, size_type destination_end)
    {
        while (source_end != source_begin) {
            source_end = calculate_next_index(source_end);
            destination_end = calculate_next_index(destination_end);

            auto source_block = calculate_block_index(source_end);
            auto source_offset = calculate_block_offset(source_end);

            auto destination_block = calculate_block_index(destination_end);
            auto destination_offset = calculate_block_offset(destination_end);

            data.blocks[destination_block][destination_offset] = std::move(data.blocks[source_block][source_offset]);
        }
    }

    template <typename T, typename Allocator>
    constexpr void deque<T, Allocator>::allocate_blocks(block_type* begin_block, block_type* end_block)
    {
        for (auto current_block = begin_block; current_block != end_block; ++current_block) {
            assert((*current_block == nullptr) && "Block is already allocated. You should use this function only with ranges of not allocated blocks");
            *current_block = std::allocator_traits<element_allocator_type>::allocate(element_allocator, block_size);
        }
    }

    template <typename T, typename Allocator>
    constexpr void deque<T, Allocator>::allocate_blocks_unsafe(block_type* begin_block, block_type* end_block)
    {
        // Should only be used if you are 100% sure that you will only overwrite garbage pointers
        // For example when you just allocated array of block pointers, but not initialized it with nulls yet
        for (auto current_block = begin_block; current_block != end_block; ++current_block) {
            *current_block = std::allocator_traits<element_allocator_type>::allocate(element_allocator, block_size);
        }
    }
    
    template <typename T, typename Allocator>
    constexpr void deque<T, Allocator>::allocate_blocks_if_not_allocated(block_type* begin_block, block_type* end_block)
    {
        for (auto current_block = begin_block; current_block != end_block; ++current_block) {
            if (*current_block == nullptr) {
                *current_block = std::allocator_traits<element_allocator_type>::allocate(element_allocator, block_size);
            }
        }
    }

    template <typename T, typename Allocator>
    constexpr bool deque<T, Allocator>::all_blocks_are_allocated() const
    {
        return std::all_of(data.blocks, data.blocks + data.blocks_count, [](block_type block){ return block != nullptr; });
    }


    

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::size_type deque<T, Allocator>::potential_capacity_back() const
    {
        if (is_memory_filled()) {
            return 0;
        }

        // Elements before begin_index in the same block are reserved for insertion to the front only
        const auto unavailable_elements = calculate_block_offset(data.begin_index);
        return capacity() - data.elements_count - unavailable_elements;
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::size_type deque<T, Allocator>::potential_capacity_front() const
    {
        if (is_memory_filled()) {
            return 0;
        }

        // Elements after end_index in the same block are reserved for insertion to the back only
        const auto unavailable_elements = data.block_size - calculate_block_offset(calculate_end_index());
        return capacity() - data.elements_count - unavailable_elements;
    }

    template <typename T, typename Allocator>
    constexpr void deque<T, Allocator>::reserve_back(size_type n)
    {
        if (potential_capacity_back() < n) { // Allocate larger array of blocks
            // Calculate required number of new blocks
            const auto extra_elements_count = n - potential_capacity_back();
            const auto extra_blocks_count = ceil_division(extra_elements_count, block_size);
            const auto new_blocks_count = std::max<size_type>(data.blocks_count + extra_blocks_count, 2);

            // Allocate new array for block and copy old data
            auto new_blocks = std::allocator_traits<block_allocator_type>::allocate(block_allocator, new_blocks_count);
            auto begin_block_index = calculate_block_index(data.begin_index);

            if (data.blocks == nullptr) {
                allocate_blocks_unsafe(new_blocks, new_blocks + new_blocks_count);
            } else if (data.begin_index < calculate_end_index()) {
                // Deque looks like this:
                //    begin      end
                //      v         v
                // [ | |#|#|#|#|#| ]
                const auto front_empty_blocks_count = begin_block_index;
                const auto back_empty_blocks_count = data.blocks_count - ceil_division(calculate_end_index(), block_size);
                const auto filled_blocks_count = data.blocks_count - (front_empty_blocks_count + back_empty_blocks_count);

                // Copy all blocks
                std::copy(data.blocks, data.blocks + data.blocks_count, new_blocks);
                allocate_blocks_if_not_allocated(new_blocks, new_blocks + front_empty_blocks_count);
                allocate_blocks_if_not_allocated(new_blocks + front_empty_blocks_count + filled_blocks_count, new_blocks + data.blocks_count);
                allocate_blocks(new_blocks + data.blocks_count, new_blocks + new_blocks_count);
            } else {
                // And here we deal with this case:
                //       end    begin
                //        v       v
                // [#|#|#| | | | |#]
                const auto back_blocks_count = data.blocks_count - begin_block_index;
                const auto front_blocks_count = ceil_division(calculate_end_index(), block_size);
                std::copy(data.blocks + begin_block_index, data.blocks + data.blocks_count, new_blocks);
                std::copy(data.blocks, data.blocks + front_blocks_count, new_blocks + back_blocks_count);
                std::copy(
                    data.blocks + front_blocks_count,
                    data.blocks + back_blocks_count,
                    new_blocks + back_blocks_count + front_blocks_count
                ); // Copy free blocks, because they can be empty but allocated and we don't want to leak
                allocate_blocks_if_not_allocated(new_blocks + back_blocks_count + front_blocks_count, new_blocks + data.blocks_count);
                allocate_blocks(new_blocks + data.blocks_count, new_blocks + new_blocks_count);
            }
            
            deallocate_blocks_array();
            data.blocks = new_blocks;
            data.blocks_count = new_blocks_count;
        } else {
            const auto free_elements_in_last_block = data.block_size - calculate_block_offset(calculate_end_index());
            auto allocated_elements = free_elements_in_last_block;

            size_type block;
            if (free_elements_in_last_block > 0) {
                block = next_block_index(calculate_block_index(calculate_end_index()));
            } else {
                block = calculate_block_index(calculate_end_index());
            }

            for (; allocated_elements < n; block = next_block_index(block), allocated_elements += data.block_size) {
                if (data.blocks[block] == nullptr) {
                    data.blocks[block] = std::allocator_traits<element_allocator_type>::allocate(element_allocator, data.block_size);
                }
            }
        }
    }

    template <typename T, typename Allocator>
    constexpr void deque<T, Allocator>::reserve_front(size_type n)
    {
        if (potential_capacity_front() < n) { // Allocate larger array of blocks
            // Calculate required number of new blocks
            const auto extra_elements_count = n - potential_capacity_front();
            const auto extra_blocks_count = ceil_division(extra_elements_count, block_size);
            const auto new_blocks_count = std::max<size_type>(data.blocks_count + extra_blocks_count, 2);

            // Allocate new array for block and copy old data
            auto new_blocks = std::allocator_traits<block_allocator_type>::allocate(block_allocator, new_blocks_count);
            auto begin_block_index = calculate_block_index(data.begin_index);
            
            if (data.blocks == nullptr) {
                allocate_blocks_unsafe(new_blocks, new_blocks + new_blocks_count);
            } else if (data.begin_index < calculate_end_index()) {
                // Deque looks like this:
                //    begin      end
                //      v         v
                // [ | |#|#|#|#|#| ]
                const auto front_empty_blocks_count = begin_block_index;
                const auto back_empty_blocks_count = data.blocks_count - ceil_division(calculate_end_index(), block_size);
                const auto filled_blocks_count = data.blocks_count - (front_empty_blocks_count + back_empty_blocks_count);

                // Copy all blocks
                std::copy(data.blocks, data.blocks + data.blocks_count, new_blocks);
                allocate_blocks_if_not_allocated(new_blocks, new_blocks + front_empty_blocks_count);
                allocate_blocks_if_not_allocated(new_blocks + front_empty_blocks_count + filled_blocks_count, new_blocks + data.blocks_count);
                allocate_blocks(new_blocks + data.blocks_count, new_blocks + new_blocks_count);
            } else {
                // And here we deal with this case:
                //       end    begin
                //        v       v
                // [#|#|#| | | | |#]
                const auto back_blocks_count = data.blocks_count - begin_block_index;
                const auto front_blocks_count = ceil_division(calculate_end_index(), block_size);
                std::copy(data.blocks + begin_block_index, data.blocks + data.blocks_count, new_blocks);
                std::copy(data.blocks, data.blocks + front_blocks_count, new_blocks + back_blocks_count);
                std::copy(
                    data.blocks + front_blocks_count,
                    data.blocks + back_blocks_count,
                    new_blocks + back_blocks_count + front_blocks_count
                ); // Copy free blocks, because they can be empty but allocated and we don't want to leak
                allocate_blocks_if_not_allocated(new_blocks + back_blocks_count + front_blocks_count, new_blocks + data.blocks_count);
                allocate_blocks(new_blocks + data.blocks_count, new_blocks + new_blocks_count);
            }
            
            deallocate_blocks_array();
            data.blocks = new_blocks;
            data.blocks_count = new_blocks_count;
        } else {
            const auto free_elements_in_first_block = calculate_block_offset(data.begin_index);
            auto allocated_elements = free_elements_in_first_block;
            for (
                auto block = previous_block_index(calculate_block_index(data.begin_index));
                allocated_elements < n;
                block = previous_block_index(block), allocated_elements += data.block_size
            ) {
                if (data.blocks[block] == nullptr) {
                    data.blocks[block] = std::allocator_traits<element_allocator_type>::allocate(element_allocator, data.block_size);
                }
            }
        }
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::size_type deque<T, Allocator>::next_block_index(size_type block_index) const
    {
        return (block_index + 1) % data.blocks_count;
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::size_type deque<T, Allocator>::previous_block_index(size_type block_index) const
    {
        return (data.blocks_count + block_index - 1) % data.blocks_count;
    }


    template <typename T, typename Allocator>
    constexpr void deque<T, Allocator>::move_construct_range(size_type source_begin, size_type source_end, size_type destination_begin)
    {
        while (source_begin != source_end) {
            auto source_block = calculate_block_index(source_begin);
            auto source_offset = calculate_block_offset(source_begin);

            auto destination_block = calculate_block_index(destination_begin);
            auto destination_offset = calculate_block_offset(destination_begin);

            std::allocator_traits<element_allocator_type>::construct(
                element_allocator,
                data.blocks[destination_block] + destination_offset,
                std::move(data.blocks[source_block][source_offset])
            );

            source_begin = calculate_next_index(source_begin);
            destination_begin = calculate_next_index(destination_begin);
        }
    }

    template <typename T, typename Allocator>
    constexpr void deque<T, Allocator>::copy_assign_range(size_type source_begin, size_type source_end, size_type destination_begin)
    {
        while (source_begin != source_end) {
            auto source_block = calculate_block_index(source_begin);
            auto source_offset = calculate_block_offset(source_begin);

            auto destination_block = calculate_block_index(destination_begin);
            auto destination_offset = calculate_block_offset(destination_begin);

            data.blocks[destination_block][destination_offset] = data.blocks[source_block][source_offset];

            source_begin = calculate_next_index(source_begin);
            destination_begin = calculate_next_index(destination_begin);
        }
    }

    template <typename T, typename Allocator>
    template <std::input_iterator InputIt>
    constexpr void deque<T, Allocator>::copy_assign_range(InputIt source_begin, InputIt source_end, size_type destination_begin)
    {
        while (source_begin != source_end) {
            auto destination_block = calculate_block_index(destination_begin);
            auto destination_offset = calculate_block_offset(destination_begin);

            data.blocks[destination_block][destination_offset] = *source_begin;

            ++source_begin;
            destination_begin = calculate_next_index(destination_begin);
        }
    }

    template <typename T, typename Allocator>
    constexpr void deque<T, Allocator>::copy_assign_range_values(size_type destination_begin, size_type destination_end, const value_type& value)
    {
        while (destination_begin != destination_end) {
            auto destination_block = calculate_block_index(destination_begin);
            auto destination_offset = calculate_block_offset(destination_begin);

            data.blocks[destination_block][destination_offset] = value;

            destination_begin = calculate_next_index(destination_begin);
        }
    }
}

#endif /* DEQUE_HPP */
