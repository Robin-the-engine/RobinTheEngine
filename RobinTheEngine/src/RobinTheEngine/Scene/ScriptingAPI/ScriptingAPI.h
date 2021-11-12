#pragma once
#include <sol/sol.hpp>

namespace RTE {

    template<typename UserType>
    void registerUserType(sol::state& lua) {
        static_assert("this function must be overriden for use with desired type" && false);
    }

}
