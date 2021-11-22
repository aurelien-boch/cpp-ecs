#ifndef INDEXED_ZIPPER_ITERATOR_HPP
#define INDEXED_ZIPPER_ITERATOR_HPP

#include <algorithm>
#include "is_sparse_array.hpp"

namespace ecs::containers
{
    template<class ...T>
    class indexed_zipper;
}

namespace ecs::iterators
{
    /**
     * @brief This class defines an iterator instantiated by the indexed zipper class. it's intended to be used in a
     * range based loop or a simple for. This iterator is the same as the zipper_iterator, the only difference is that
     * this one provides you the index of the entity as the first parameter the tuple when you dereference it.
     * @tparam Containers This variadic template refers to the types to bind the iterator.
     */
    template<class ...Containers>
    class indexed_zipper_iterator
    {
            template<class Container>
            using iterator_t = typename Container::iterator;

            template<class Container>
            using it_reference_t = decltype(std::declval<typename iterator_t<Container>::reference>().value());

        public:
            using value_type = std::tuple<std::size_t, it_reference_t<Containers>...>;
            using reference = value_type &;
            using pointer = void;
            using difference_type = std::size_t;
            using iterator_category = std::input_iterator_tag;
            using iterator_tuple = std::tuple<iterator_t<Containers>...>;

            friend ecs::containers::indexed_zipper<Containers ...>;

            indexed_zipper_iterator(indexed_zipper_iterator const &z) :
                _current(z._current),
                _max(z._max),
                _idx(z._idx)
            {}

            const indexed_zipper_iterator &operator++() //TODO fix this magic operator
            {
                if (_max == 0)
                    return (*this);
                if (_idx == _max - 1) {
                    _incr_all(_seq);
                    _idx++;
                }
                else if (_idx < _max - 1)
                    do {
                        _incr_all(_seq);
                        _idx += 1;
                    } while (!_all_set(_seq) && _idx < _max - 1);
                if (_idx == _max - 1 && !_all_set(_seq)) {
                    _incr_all(_seq);
                    _idx++;
                }
                return (*this);
            }

            indexed_zipper_iterator operator++(int)
            {
                indexed_zipper_iterator<Containers...> old = *this;
                operator++();
                return (old);
            }

            value_type operator*()
            {
                return (_to_value(_seq));
            }

            value_type operator->()
            {
                return (_to_value(_seq));
            }

            friend inline bool operator==(indexed_zipper_iterator const &lhs, indexed_zipper_iterator const &rhs)
            {
                return ((lhs._max - lhs._idx) == (rhs._max - rhs._idx));
            }

            friend inline bool operator!=(indexed_zipper_iterator const &lhs, indexed_zipper_iterator const &rhs)
            {
                return ((lhs._max - lhs._idx) != (rhs._max - rhs._idx));
            }

        private:

            iterator_tuple _current;

            std::size_t _max;

            std::size_t _idx;

            static constexpr std::index_sequence_for<Containers ...> _seq{};

            template<size_t ... Is>
            void _incr_all(std::index_sequence<Is ...>)
            {
                ((++std::get<Is>(_current)), ...);
            }

            template<size_t ... Is>
            [[nodiscard]] bool _all_set(std::index_sequence<Is ...>)
            {
                return (((*std::get<Is>(_current)) != std::nullopt) && ...);
            }

            template<size_t ... Is>
            [[nodiscard]] value_type _to_value(std::index_sequence<Is ...>)
            {
                return std::tie(_idx, std::get<Is>(_current)->value()...);
            }

            indexed_zipper_iterator(iterator_tuple const &it_tuple, std::size_t max) :
                _current(it_tuple),
                _max(max),
                _idx(0)
            {
                if (max > 0 && !_all_set(_seq))
                    this->operator++();
            }
    };
}

#endif //INDEXED_ZIPPER_ITERATOR_HPP
