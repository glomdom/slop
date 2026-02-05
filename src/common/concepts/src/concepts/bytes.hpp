#include <concepts>
#include <ranges>

template <typename T>
concept Byte = (sizeof(T) == 1) && !std::same_as<std::remove_cvref_t<T>, bool>;

template <typename T>
concept ContiguousBytes = std::ranges::contiguous_range<T> && Byte<std::ranges::range_value_t<T>>;