#ifndef TOY_SDL_DEQUE_DATA_HPP
#define TOY_SDL_DEQUE_DATA_HPP

#include <cmath>

template <typename value_type, typename size_type>
constexpr size_type calculate_block_size()
{
    // Should work like clang strategy
    // When sizeof(value_type) < 256 you get maximum number of elements you can fit in 4096 bytes
    // Otherwise you always get 16 elements
    return std::max(4096 / sizeof(value_type), std::size_t{ 16 });
}

template <typename value_type, typename size_type>
struct deque_data
{
    using block_type = value_type*; // Chunk of memory with size = block_size
    constexpr static size_type block_size = calculate_block_size<value_type, size_type>();

    // Circular array of blocks with size = blocks_count
    block_type* blocks { nullptr };
    size_type blocks_count { 0 };
    // Index like in array of size block_count * block_size
    // Range is exclusive - [begin_index, begin_index + elements_count)
    // And supports wrapping around
    size_type begin_index { 0 }; // Maybe should change to signed type
    size_type elements_count { 0 };
};

#endif /* TOY_SDL_DEQUE_DATA_HPP */
