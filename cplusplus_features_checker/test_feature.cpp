#include <iostream>
#include <array>
#include <string>
#include <algorithm>
#include <map>
#include "attributes.hpp"
#include "language_features.hpp"
#include "library_features.hpp"


int main() {
    for (auto& [attr_name, has_attr] : attributes) {
        std::cout << attr_name << " : " << (has_attr?"has":"none") << std::endl;
    }
    for (auto& [name, value] : language_features) {
        std::cout << name << " : " << value << std::endl;
    }
    for (auto& [name, value] : library_features) {
        std::cout << name << " : " << value << std::endl;
    }
    return 0;
}