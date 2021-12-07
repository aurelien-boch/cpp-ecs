#include <memory>

#if defined(__GNUC__)
    #include "cxxabi.h"
#endif

#include "exceptions/component_already_registered_exception.hpp"

namespace ecs::exceptions
{
    component_already_registered_exception::component_already_registered_exception(
        std::type_info const &component,
        std::vector<std::type_index> const &registeredComponents)
    {
        _errorMessage = "Component " +
                        _getName(component) +
                        " already registered. Registered components:";

        for (auto const &e : registeredComponents)
            _errorMessage += "\n\t- " + _getName(e);
    }

    const char *component_already_registered_exception::what() const noexcept
    {
        return _errorMessage.c_str();
    }

    std::string component_already_registered_exception::_getName(std::type_index const &index)
    {
        #if defined(__GNUC__)
            int status;
                auto realName = abi::__cxa_demangle(
                    index.name(),
                    nullptr,
                    nullptr,
                    &status);

                std::string res(realName);
                std::free(realName);
        #else
            std::string res(index.name());
        #endif

        return (res);
    }
}
