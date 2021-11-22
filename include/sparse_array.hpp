#ifndef SPARSE_ARRAY_HPP
#define SPARSE_ARRAY_HPP

#include <vector>
#include <optional>
#include <algorithm>
#include <stdexcept>

namespace ecs::containers
{
    /**
     * @brief This class refers to an array of Components.
     * @tparam Component This template refers to the type of the component.
     */
    template<typename Component>
    class sparse_array
    {
        public:
            using value_type = std::optional<Component>;

            using reference_type = value_type &;

            using const_reference_type = value_type const &;

            using container_type = std::vector<value_type>;

            using size_type = typename container_type::size_type;

            using iterator = typename container_type::iterator;

            using const_iterator = typename container_type::const_iterator;

            sparse_array():
                _data{}
            {
                char a[3];

                a[15] = 'h';
                int k = 0x7fffffff;
                k += 10;
            }

            sparse_array(sparse_array const &other) = default;

            sparse_array(sparse_array &&other) noexcept = default;

            ~sparse_array() = default;

            sparse_array &operator=(sparse_array const &other) = default;

            sparse_array &operator=(sparse_array &&other) noexcept = default;

            [[nodiscard]] reference_type operator[](size_t index)
            {
                return _data[index];
            }

            [[nodiscard]] const_reference_type operator[](size_t index) const
            {
                return _data[index];
            }

            /**
             * @brief This method returns an iterator to the beginning of the internal vector.
             */
            [[nodiscard]] iterator begin()
            {
                return _data.begin();
            }

            /**
             * @brief This method returns a const iterator to the beginning of the internal vector.
             */
            [[nodiscard]] const_iterator begin() const
            {
                return _data.begin();
            }

            /**
             * @brief This method returns a const iterator to the beginning of the internal vector.
             */
            [[nodiscard]] const_iterator cbegin() const
            {
                return _data.cbegin();
            }

            /**
             * @brief This method returns an iterator to the end of the internal vector.
             */
            [[nodiscard]] iterator end()
            {
                return _data.end();
            }

            /**
             * @brief This method returns a const iterator to the end of the internal vector.
             */
            [[nodiscard]] const_iterator end() const
            {
                return _data.end();
            }

            /**
             * @brief This method returns a const iterator to the end of the internal vector.
             */
            [[nodiscard]] const_iterator cend() const
            {
                return _data.cend();
            }

            /**
             * @brief This method returns the size of the sparse_array.
             * @return The size of the sparse_array as a ContainerSizeType.
             */
            [[nodiscard]] size_type size() const
            {
                return _data.size();
            }

            /**
             * @brief This method inserts a component into the sparse_array at a given position and assigns it a
             * given value.
             * @param pos The position of the element to be assigned.
             * @param component The value to assign to the component.
             * @return A reference to the element stored in the sparse array.
             */
            reference_type insert_at(size_type pos, Component const &component)
            {
                if (pos >= _data.size())
                    _data.resize(pos + 1);
                _data[pos] = component;
                return (_data[pos]);
            }

            /**
             * @brief This method inserts a component into the sparse_array at a given position and moves it a
             * given value.
             * @param pos The position of the element to be assigned.
             * @param component The value to assign to the component.
             * @return A reference to the element stored in the sparse array.
             */
            reference_type insert_at(size_type pos, Component &&component)
            {
                if (pos >= _data.size())
                    _data.resize(pos + 1);
                _data[pos] = std::forward<Component>(component);
                return (_data[pos]);
            }

            /**
             * @brief This method constructs a component at a given pos.
             * @tparam Params This variadic template refers to the type of the parameter to pass to the constructor.
             * @param [in] pos This parameter refers to the position of the component to be build.
             * @param [in] parameters This parameter refers to the parameters to pass to the constructor.
             * @return A reference to the element stored in the sparse array.
             */
            template<class ... Params>
            reference_type emplace_at(size_type pos, Params &&...parameters)
            {
                auto allocator = _data.get_allocator();
                using traits = std::allocator_traits<decltype(allocator)>;

                if (pos >= _data.size())
                    _data.resize(pos + 1);
                traits::destroy(allocator, &_data[pos]);
                traits::construct(allocator, &_data[pos], std::in_place, std::forward<Params>(parameters)...);
                return (_data[pos]);
            }

            /**
             * @brief This method erases an element from the sparse_array.
             * @param [in] pos The position of the element to erase.
             */
            void erase(size_type pos)
            {
                _data.erase(pos);
            }

            /**
             * @brief This method returns the index of a component.
             * @param [in] val This parameter refers to the value to search for in the array
             * @return The index of the component. Returns -1 if the component is not found.
             * @throw If value is not found, method throws an std::out_of_range
             */
            size_type getIndex(value_type const &val) const
            {
                auto const &it = std::find_if(_data.begin(),  _data.end(), [&val] (value_type const &needle) {
                    return (std::addressof(val) == std::addressof(needle));
                });

                if (it == _data.end())
                    throw std::out_of_range("Value not found");
                return (it - _data.begin());
            }

        private :
            container_type _data{};
    };
}

#endif //SPARSE_ARRAY_HPP
