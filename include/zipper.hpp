#ifndef ZIPPER_HPP
#define ZIPPER_HPP

#ifdef _WIN32
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
#endif

#include <algorithm>

#include "is_sparse_array.hpp"
#include "zipper_iterator.hpp"
#include "_internal.hpp"


namespace ecs::containers
{
    /**
     * @brief Class that allows you to instantiate iterators over components. Iterator will iterate over all entity
     * that has ALL components. If one component is missing form the entity, the latter will be skipped.
     * @warning If any of the sparse_array is resized (add entity, add component to an entity that has an index greater
     * than the size of the sparse_array, etc...) the zipper and all its iterators are invalided. (if you continue using
     * it, the behavior is \b UNDEFINED and crashes / \b SIGSEGV may occur).
     * @tparam Containers This template parameter refers to the components you want to iterate over.
     */
    template<class ... Containers>
    class zipper
    {
        static_assert((assertion::is_sparse_array_v<Containers> && ...), "Containers must be sparse_array.");

        public:
            using iterator = iterators::zipper_iterator<Containers ...>;
            using iterator_tuple = typename iterator::iterator_tuple;

            /**
             * @param [in | out] cs This parameter refers to the sparse_array containing the component you specified in
             * template.
             */
            ExportSymbol explicit zipper(Containers &... cs) :
                _size(_computeSize(cs...)),
                _begin(cs.begin()...),
                _end(_compute_end(cs..., _size))
            {}

            /**
             * @brief This method returns a zipper_iterator referring to the beginning of all SparseArrays.
             * @return iterator.
             */
            [[nodiscard]] ExportSymbol iterator begin() noexcept
            {
                return iterator(_begin, _size);
            }

            /**
             * @brief This method returns a zipper_iterator referring to the end of all SparseArrays.
             * @return iterator.
             */
            [[nodiscard]] ExportSymbol iterator end() noexcept
            {
                return iterator(_end, 0);
            }

        private:
            size_t _size;

            iterator_tuple _begin;

            iterator_tuple _end;

            [[nodiscard]] ExportSymbol static size_t _computeSize(Containers &... containers)
            {
                return (std::min)(
                    { containers.size()... }
               );
            }

            [[nodiscard]] ExportSymbol static iterator_tuple _compute_end(Containers &... containers, std::size_t minSize)
            {
                return std::tuple((containers.begin() + minSize)...);
            }
    };
}

#endif //ZIPPER_HPP
