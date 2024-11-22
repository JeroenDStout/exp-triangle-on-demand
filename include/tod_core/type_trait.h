#pragma once

namespace tod {

    // Gets the element type of a vector, the return type of (vector-instance)[int]
    template<class T>
    struct vector_element_type {
      using value = std::remove_reference_t<decltype(std::declval<T>()[0])>;
    };

    template<class T>
    using vector_element_type_v = vector_element_type<T>::value;

}