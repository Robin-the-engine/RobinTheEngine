#pragma once
#include <unordered_map>

namespace BlackBoard {
    template <typename T>
    T fromString (std::string) {
        static_assert(false, "you should implement this function for your type");
    }

    template <>
    int fromString(std::string s) {
        int number;
        std::from_chars(s.data(), s.data() + s.size(), number);
        return number;
    }

    template <>
    float fromString(std::string s) {
        float number;
        std::from_chars(s.data(), s.data() + s.size(), number);
        return number;
    }

    template <>
    double fromString(std::string s) {
        double number;
        std::from_chars(s.data(), s.data() + s.size(), number);
        return number;
    }

}

struct BlackBoardImpl final {


    //std::unordered_map<std::string, >
};
