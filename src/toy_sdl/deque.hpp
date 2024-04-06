#ifndef TOY_SDL_DEQUE_HPP
#define TOY_SDL_DEQUE_HPP

#include <memory>

namespace my
{
    template <typename value_type, typename size_type>
    constexpr size_type calculate_block_size()
    {
        // Should work like clang strategy
        // When sizeof(value_type) < 256 you get maximum number of elements you can fit in 4096 bytes
        // Otherwise you always get 16 elements
        return std::max(4096 / sizeof(value_type), 16);
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
        template <class InputIt>
        constexpr deque(InputIt first, InputIt last, const Allocator& allocator = Allocator());
        constexpr deque(std::initializer_list<T> init_list, const Allocator& allocator = Allocator());

        // Rule of 5
        // constexpr deque(const deque& other);
        // constexpr deque(const deque& other, const Allocator& allocator);
        // constexpr deque& operator=(deque other);

        // constexpr deque(deque&& other);
        // constexpr deque(deque&& other, const Allocator& allocator);
        // constexpr deque& operator=(deque&& other);
        
        // ~deque();

    private:
        constexpr static size_type block_size = calculate_block_size<T, size_type>(); // Size as number of elements not bytes
        // Chunk of memory with size = block_size
        using block_type = element_type*;

        using element_allocator = allocator_type;
        using block_allocator = std::allocator_traits<allocator_type>::rebind_alloc<block_type>;

        element_allocator element_allocator { };
        block_allocator block_allocator { };
        
        // Circular array of blocks with size = blocks_count
        block_type* blocks { nullptr };
        size_type blocks_count { 0 };
        // Index like in array of size block_count * block_size
        size_type first_element_index { 0 }; // Maybe should change to signed type
        size_type last_element_index { 0 };
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
    template <class InputIt>
    constexpr deque<T, Allocator>::deque(InputIt first, InputIt last, const Allocator& allocator)
    {
        // TODO:
    }

    template <typename T, typename Allocator>
    constexpr deque<T, Allocator>::deque(std::initializer_list<T> init_list, const Allocator& allocator) :
        deque(std::begin(init_list), std::end(init_list), allocator)
    {

    }
}

#endif /* TOY_SDL_DEQUE_HPP */
