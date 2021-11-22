#ifndef ZIPPER_ITERATOR_HPP
#define ZIPPER_ITERATOR_HPP

#include <tuple>
#include <optional>

namespace ecs::containers
{
    template<class ...T>
    class zipper;
}

namespace ecs::iterators
{
    /**
     * @brief This class defines an iterator instantiated by the zipper class. it's intended to be used in a range based
     * loop or a simple for.
     * @tparam Containers This variadic template refers to the types to bind the iterator.
     */
    template<class ...Containers>
    class zipper_iterator
    {
            template<class Container>
            using iterator_t = typename Container::iterator;

            template<class Container>
            using it_reference_t = decltype(std::declval<typename iterator_t<Container>::reference>().value());

        public:
            using value_type = std::tuple<it_reference_t<Containers>...>;
            using reference = value_type &;
            using pointer = void;
            using difference_type = std::size_t;
            using iterator_category = std::input_iterator_tag;
            using iterator_tuple = std::tuple<iterator_t<Containers>...>;

            friend ecs::containers::zipper<Containers ...>;

            zipper_iterator(zipper_iterator const &z) noexcept :
                _max(z._max),
                _idx(z._idx),
                _current(z._current)
            {}

            const zipper_iterator &operator++() //TODO fix this magic
            {
                if (_max == 0)
                    return (*this);
                if (_idx == _max - 1) {
                    _incrAll(_seq);
                    _idx++;
                }
                else if (_idx < _max - 1)
                    do {
                        _incrAll(_seq);
                        _idx += 1;
                    } while (!_allSet(_seq) && _idx < _max - 1);
                if (_idx == _max - 1 && !_allSet(_seq)) {
                    _incrAll(_seq);
                    _idx++;
                }
                return (*this);
            }

            zipper_iterator operator++(int)
            {
                zipper_iterator<Containers...> old = *this;
                operator++();
                return (old);
            }

            value_type operator*()
            {
                return (_toValue(_seq));
            }

            value_type operator->()
            {
                return (_toValue(_seq));
            }

            friend inline bool operator==(zipper_iterator const &lhs, zipper_iterator const &rhs)
            {
                return ((lhs._max - lhs._idx) == (rhs._max - rhs._idx));
            }

            friend inline bool operator!=(zipper_iterator const &lhs, zipper_iterator const &rhs)
            {
                return ((lhs._max - lhs._idx) != (rhs._max - rhs._idx));
            }

        private:

            iterator_tuple _current;

            std::size_t _max;

            std::size_t _idx{};

            static constexpr std::index_sequence_for<Containers ...> _seq{};

            template<size_t ... Is>
            void _incrAll(std::index_sequence<Is ...>)
            {
                ((++std::get<Is>(_current)), ...);
            }

            template<size_t ... Is>
            [[nodiscard]] bool _allSet(std::index_sequence<Is ...>)
            {
                return (((*std::get<Is>(_current)) != std::nullopt) && ...);
            }

            template<size_t ... Is>
            [[nodiscard]] value_type _toValue(std::index_sequence<Is ...>)
            {
                return std::tie(std::get<Is>(_current)->value()...);
            }

            zipper_iterator(iterator_tuple const &it_tuple, std::size_t max) :
                _current(it_tuple),
                _max(max)
            {
                if (_max > 0 && !_allSet(_seq))
                    this->operator++();
            }
    };
}

#endif //ZIPPER_ITERATOR_HPP
