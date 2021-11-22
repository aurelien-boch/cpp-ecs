set(
    SRCS
        ${CMAKE_CURRENT_LIST_DIR}/entity.cpp
        ${CMAKE_CURRENT_LIST_DIR}/registry.cpp
        ${CMAKE_CURRENT_LIST_DIR}/exceptions/component_not_registered_exception.cpp
        ${CMAKE_CURRENT_LIST_DIR}/exceptions/component_already_registered_exception.cpp
        PARENT_SCOPE
)
