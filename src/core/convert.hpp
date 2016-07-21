#pragma once

#include <seqan/sequence.h>

#include <sstream>

namespace core {
    template<typename T1, typename T2>
    T2 convert(T1 obj) {
        std::stringstream ss;
        ss << obj;
        T2 converted;
        ss >> converted;
        return converted;
    };

    std::string dna5String_to_string(seqan::Dna5String);

    template<typename T>
    T string_to_number(std::string s) {
        std::stringstream ss;
        ss << s;
        T n;
        ss >> n;
        return n;
    }
}