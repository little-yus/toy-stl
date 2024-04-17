#ifndef TOY_SDL_DEQUE_HPP
#define TOY_SDL_DEQUE_HPP

#include <memory>
#include <cmath>
#include <cassert>
#include <stdexcept>

namespace my
{
    template <typename value_type, typename size_type>
    constexpr size_type calculate_block_size()
    {
        // Should work like clang strategy
        // When sizeof(value_type) < 256 you get maximum number of elements you can fit in 4096 bytes
        // Otherwise you always get 16 elements
        return std::max(4096 / sizeof(value_type), std::size_t{ 16 });
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
        
        // Iterator types
        // TODO
        // using iterator = ;
        // using const_iterator = ;
        // using reverse_iterator = std::reverse_iterator<iterator>;
        // using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        // Constructors
        constexpr deque(); // Standard says nothing about nothrow so we assume it can throw https://stackoverflow.com/questions/15191741/does-default-constructor-of-deque-throw
        constexpr explicit deque(const Allocator& allocator);
        constexpr deque(size_type count, const_reference value, const Allocator& allocator = Allocator());
        constexpr explicit deque(size_type count, const Allocator& allocator = Allocator());
        template <typename InputIt>
        constexpr deque(InputIt first, InputIt last, const Allocator& allocator = Allocator());
        constexpr deque(std::initializer_list<T> init_list, const Allocator& allocator = Allocator());

        // Rule of 5
        // constexpr deque(const deque& other);
        // constexpr deque(const deque& other, const Allocator& allocator);
        // constexpr deque& operator=(deque other);

        // constexpr deque(deque&& other);
        // constexpr deque(deque&& other, const Allocator& allocator);
        // constexpr deque& operator=(deque&& other);
        
        constexpr ~deque();

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

    private:
        constexpr void grow_capacity();
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

        constexpr bool adding_back_element_would_force_move() const;
        constexpr bool adding_front_element_would_force_move() const;

        constexpr void destroy_all_elements();
        constexpr void deallocate_all_blocks();
        constexpr void deallocate_blocks_array();

        constexpr static size_type block_size = calculate_block_size<T, size_type>(); // Size as number of elements not bytes
        // Chunk of memory with size = block_size
        using block_type = value_type*;

        using element_allocator_type = allocator_type;
        using block_allocator_type = typename std::allocator_traits<allocator_type>::template rebind_alloc<block_type>;

        element_allocator_type element_allocator { };
        block_allocator_type block_allocator { }; // Name is a bit confusing, because it allocated arrays of pointers to blocks and not actual blocks
        
        // Circular array of blocks with size = blocks_count
        block_type* blocks { nullptr };
        size_type blocks_count { 0 };
        // Index like in array of size block_count * block_size
        // Range is exclusive - [begin_index, begin_index + elements_count)
        // And supports wrapping around
        size_type begin_index { 0 }; // Maybe should change to signed type
        size_type elements_count { 0 };
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
        element_allocator(allocator),
        block_allocator(allocator)
    {
        // TODO:
        // resize(count, value);
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::deque(size_type count, const Allocator& allocator)
    {
        // TODO:
        // resize(count);
    }

    template <typename T, typename Allocator>
    template <typename InputIt>
    constexpr deque<T, Allocator>::deque(InputIt first, InputIt last, const Allocator& allocator)
    {
        // TODO:
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::deque(std::initializer_list<T> init_list, const Allocator& allocator) :
        deque(std::begin(init_list), std::end(init_list), allocator)
    {

    }


    // Rule of 5
    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::~deque()
    {
        destroy_all_elements();
        deallocate_all_blocks();
        deallocate_blocks_array();
    }


    // Element access
    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::reference deque<T, Allocator>::operator[](size_type index)
    {
        const auto physical_index = calculate_next_index(begin_index, index);
        const auto block_index = calculate_block_index(physical_index);
        const auto block_offset = calculate_block_offset(physical_index);
        return blocks[block_index][block_offset];
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::const_reference deque<T, Allocator>::operator[](size_type index) const
    {
        const auto physical_index = calculate_next_index(begin_index, index);
        const auto block_index = calculate_block_index(physical_index);
        const auto block_offset = calculate_block_offset(physical_index);
        return blocks[block_index][block_offset];
    }


    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::reference deque<T, Allocator>::at(size_type index)
    {
        if (index >= elements_count) {
            throw std::out_of_range("Invalid element index");
        }

        return (*this)[index];
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::const_reference deque<T, Allocator>::at(size_type index) const
    {
        if (index >= elements_count) {
            throw std::out_of_range("Invalid element index");
        }

        return (*this)[index];
    }


    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::reference deque<T, Allocator>::front()
    {
        const auto block_index = calculate_block_index(begin_index);
        const auto block_offset = calculate_block_offset(begin_index);

        return blocks[block_index][block_offset];
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::const_reference deque<T, Allocator>::front() const
    {
        const auto block_index = calculate_block_index(begin_index);
        const auto block_offset = calculate_block_offset(begin_index);
        
        return blocks[block_index][block_offset];
    }


    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::reference deque<T, Allocator>::back()
    {
        const auto last_element_index = calculate_previous_index(calculate_end_index());
        auto block_index = calculate_block_index(last_element_index);
        auto block_offset = calculate_block_offset(last_element_index);

        return blocks[block_index][block_offset];
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::const_reference deque<T, Allocator>::back() const
    {
        const auto last_element_index = calculate_previous_index(calculate_end_index());
        auto block_index = calculate_block_index(last_element_index);
        auto block_offset = calculate_block_offset(last_element_index);

        return blocks[block_index][block_offset];
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
        return elements_count;
    }


    // Modifiers
    template <typename T, typename Allocator>
    constexpr void deque<T, Allocator>::clear() noexcept
    {
        destroy_all_elements();
        // Begin index remains the same to utilize memory of already allocated blocks
        elements_count = 0;
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
        if (is_memory_filled() || adding_back_element_would_force_move()) {
            grow_capacity();
        }
        
        auto end_index = calculate_end_index();
        auto end_block = calculate_block_index(end_index);
        auto end_offset = calculate_block_offset(end_index);

        // Allocate if we step into unallocated block
        // Should not overwrite old blocks
        if (blocks[end_index] == nullptr) {
            blocks[end_index] = std::allocator_traits<element_allocator_type>::allocate(element_allocator, block_size);
        }
        std::allocator_traits<element_allocator_type>::construct(
            element_allocator,
            blocks[end_block] + end_offset,
            std::forward<Args>(args) ...
        );

        ++elements_count;

        return blocks[end_block][end_offset];
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
        if (is_memory_filled() || adding_front_element_would_force_move()) {
            grow_capacity();
        }
        
        auto new_begin_index = calculate_previous_index(begin_index);
        auto new_begin_block = calculate_block_index(new_begin_index);
        auto new_begin_offset = calculate_block_offset(new_begin_index);

        // Allocate if we step into unallocated block
        // Should not overwrite old blocks
        if (blocks[new_begin_block] == nullptr) {
            blocks[new_begin_block] = std::allocator_traits<element_allocator_type>::allocate(element_allocator, block_size);
        }
        std::allocator_traits<element_allocator_type>::construct(
            element_allocator,
            blocks[new_begin_block] + new_begin_offset,
            std::forward<Args>(args) ...
        );

        begin_index = new_begin_index;
        ++elements_count;

        return blocks[new_begin_block][new_begin_offset];
    }


    template <typename T, typename Allocator>
    constexpr void deque<T, Allocator>::pop_back()
    {
        assert((elements_count > 0) && "Trying to remove last element of empty deque is undefined behavior");

        const auto last_element_index = calculate_previous_index(calculate_end_index());
        const auto last_element_block = calculate_block_index(last_element_index);
        const auto last_element_offset = calculate_block_offset(last_element_index);

        std::allocator_traits<element_allocator_type>::destroy(
            element_allocator,
            blocks[last_element_block] + last_element_offset
        );

        --elements_count;
    }

    template <typename T, typename Allocator>
    constexpr void deque<T, Allocator>::pop_front()
    {
        assert((elements_count > 0) && "Trying to remove first element of empty deque is undefined behavior");

        auto begin_block = calculate_block_index(begin_index);
        auto begin_offset = calculate_block_offset(begin_index);

        std::allocator_traits<element_allocator_type>::destroy(
            element_allocator,
            blocks[begin_block] + begin_offset
        );

        begin_index = calculate_next_index(begin_index);
        --elements_count;
    }


    // Implementation details
    template <typename T, typename Allocator>
    constexpr void deque<T, Allocator>::grow_capacity()
    {
        size_type new_blocks_count;
        if (blocks_count == 0) {
            new_blocks_count = 2; // Minimum 2 blocks, number is arbitrary
        } else {
            new_blocks_count = blocks_count * 2;
        }

        auto new_blocks = std::allocator_traits<block_allocator_type>::allocate(block_allocator, new_blocks_count);
        auto begin_block_index = calculate_block_index(begin_index);
        auto begin_block_offset = calculate_block_offset(begin_index);
        // We copy blocks before begin block to the start of the new memory
        std::copy(blocks, blocks + begin_block_index, new_blocks);
        // And all blocks after (including begin block) to the end
        // Backwards because calculating d_first is more complicated than d_last
        std::copy_backward(blocks + begin_block_index, blocks + blocks_count, new_blocks + new_blocks_count);

        auto new_begin_block_index = new_blocks_count - (blocks_count - begin_block_index);

        // Don't forget to fill the gap between the two with nullptrs
        std::fill(new_blocks + begin_block_index, new_blocks + new_begin_block_index, nullptr);

        // Clear block pointers array
        std::allocator_traits<block_allocator_type>::deallocate(block_allocator, blocks, blocks_count);

        begin_index = calculate_element_index(new_begin_block_index, begin_block_offset) % (new_blocks_count * block_size);
        blocks = new_blocks;
        blocks_count = new_blocks_count;
        // elements_count is not changed
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::size_type deque<T, Allocator>::capacity() const
    {
        return block_size * blocks_count;
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::size_type deque<T, Allocator>::calculate_end_index() const
    {
        assert((elements_count < capacity()) && "Memory is filled. End index would end up equal to begin index. You should handle this case separately using is_memory_filled");
        return (begin_index + elements_count) % capacity();
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
    constexpr bool deque<T, Allocator>::adding_front_element_would_force_move() const
    {
        // Blocks like this one are ok:
        //    begin      end
        //      v         v
        // [ | |#|#|#|#|#| ]

        // But with block like this it is not possible to add new blocks without moving some elements
        //     end      begin
        //      v         v
        // [#|#|#| | | | |#]

        auto new_begin_index = calculate_previous_index(begin_index);
        auto end_index = calculate_end_index();

        auto new_begin_block = calculate_block_index(new_begin_index);
        auto end_block = calculate_block_index(end_index);

        return (new_begin_index > end_index) && (new_begin_block == end_block);
    }

    template <typename T, typename Allocator>
    constexpr bool deque<T, Allocator>::adding_back_element_would_force_move() const
    {
        auto end_index = calculate_end_index();
        auto new_end_index = calculate_next_index(end_index);

        auto new_end_block = calculate_block_index(new_end_index);
        auto begin_block = calculate_block_index(begin_index);

        return (new_end_index < begin_index) && (new_end_block == begin_block);
    }

    template <typename T, typename Allocator>
    constexpr bool deque<T, Allocator>::is_memory_filled() const
    {
        return elements_count == capacity();
    }

    template <typename T, typename Allocator>
    constexpr void deque<T, Allocator>::destroy_all_elements()
    {
        auto current_index = begin_index;

        for (size_type i = 0; i < elements_count; ++i) {
            const auto current_block_index = calculate_block_index(current_index);
            const auto current_block_offset = calculate_block_offset(current_index);

            std::allocator_traits<element_allocator_type>::destroy(
                element_allocator,
                blocks[current_block_index] + current_block_offset
            );

            current_index = calculate_next_index(current_index);
        }
    }

    template <typename T, typename Allocator>
    constexpr void deque<T, Allocator>::deallocate_all_blocks()
    {
        for (size_type i = 0; i < blocks_count; ++i) {
            if (blocks[i] != nullptr) {
                std::allocator_traits<element_allocator_type>::deallocate(element_allocator, blocks[i], block_size);
            }
        }
    }

    template <typename T, typename Allocator>
    constexpr void deque<T, Allocator>::deallocate_blocks_array()
    {
        std::allocator_traits<block_allocator_type>::deallocate(block_allocator, blocks, blocks_count);
    }
}

#endif /* TOY_SDL_DEQUE_HPP */
