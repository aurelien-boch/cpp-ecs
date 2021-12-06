#ifndef COMPONENT_NOT_REGISTERED_EXCEPTION_HPP
#define COMPONENT_NOT_REGISTERED_EXCEPTION_HPP


#include <exception>
#include <string>
#include <typeindex>
#include <any>
#include <vector>

#include "_internal.hpp"

namespace ecs::exceptions
{
    /**
     * @brief This exception will be thrown when a component is called but not registered before.
     */
    class component_not_registered_exception : public std::exception
    {
        public:
            /**
             * @param [in] component This parameter refers to the component that was called.
             * @param [in] registeredComponents This parameter refers to a vector of all registered components.
             */
            ExportSymbol component_not_registered_exception(
                std::type_info const &component,
                std::vector<std::type_index> const &registeredComponents);

            /**
             * @brief Returns a C-style character string describing the general cause of the current error.
             */
            [[nodiscard]] ExportSymbol const char *what() const noexcept override;

            ExportSymbol ~component_not_registered_exception() override = default;

        private:
            static ExportSymbol std::string _getName(std::type_index const &index);

            std::string _errorMessage{};
    };
}

#endif //COMPONENT_NOT_REGISTERED_EXCEPTION_HPP
