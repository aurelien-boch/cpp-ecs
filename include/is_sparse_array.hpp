#ifndef ISSPASEARRAY_HPP
#define ISSPASEARRAY_HPP

#include "sparse_array.hpp"

namespace ecs::assertion
{
    /**
     * @brief The is_sparse_array struct contains a static field named value that is true if the template is a
     * containers::sparse_array<T>. Otherwise the field is equals to false.
     * @tparam T This template parameter refers to the type to check.
     */
    template<class T>
    struct is_sparse_array : std::false_type {};


    template<class T>
    struct is_sparse_array<containers::sparse_array<T>> : std::true_type {};

    template<class T>
    constexpr inline bool is_sparse_array_v = is_sparse_array<T>::value;
}

#endif //ISSPASEARRAY_HPP
