#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>

using namespace std::literals;

template<class Op>
inline auto for_each_line(std::ifstream& file, Op&& op){
    std::vector<std::invoke_result_t<Op, std::string>> res;
    while (1) {
        std::string line;
        if (std::getline(file, line)) {
            res.emplace_back(op(std::move(line)));
        }
        else {
            break;
        }
    }
    return res;
}

template<class T>
inline T equality(T t) {
    return t;
}


class language_feature {
public:
    language_feature(std::string name) : name{name}{}
    std::string test_macro() {
        return "__cpp_"s + name;
    }
private:
    std::string name;
};

class library_feature {
public:
    library_feature(std::string name) : name{name}{}
    std::string test_macro() {
        return "__cpp_lib_"s + name;
    }
private:
    std::string name;
};

std::vector<std::string> get_attributes() {
    std::vector<std::string> attributes;
    if (std::ifstream file{"attributes"}; file.is_open()) {
        attributes = for_each_line(file, equality<std::string>);
    }

    return attributes;
}

auto get_language_features() {
    std::vector<language_feature> features;
    if (std::ifstream file{"language_features"}; file.is_open()) {
        features = for_each_line(file, [](std::string line){
            return language_feature{line};
        });
    }
    return features;
}

auto get_library_features() {
    std::vector<library_feature> features;
    if (std::ifstream file{"library_features"}; file.is_open()) {
        features = for_each_line(file, [](std::string line) {
            return library_feature{line};
        });
    }
    return features;
}

void process_attributes() {
    if (std::ofstream file{"attributes.hpp"}; file.is_open()) {
        file << "std::map<std::string, long> attributes{" << std::endl;
        auto attributes = get_attributes();
        std::for_each(std::begin(attributes), std::end(attributes),
        [&file=file](std::string attr) {
            file << "{\"" << attr << "\", " << std::endl
                << "#if __has_cpp_attribute(" << attr << ")" << std::endl
                << "1" << std::endl
                << "#else" << std::endl
                << "0" << std::endl
                << "#endif" << std::endl
                << "}," << std::endl;
        });
        file << "};" << std::endl;
    }
}

void process_language_features() {
    if (std::ofstream file{"language_features.hpp"}; file.is_open()) {
        file << "std::map<std::string, long> language_features{" << std::endl;
        auto features = get_language_features();
        std::for_each(std::begin(features), std::end(features),
        [&file=file](language_feature feature) {
            auto macro = feature.test_macro();
            file << "{\"" << macro << "\", " << std::endl
            << "#ifdef " << macro << std::endl
            << macro << std::endl
            << "#else" << std::endl
            << "0" << std::endl
            << "#endif" << std::endl
            << "}," << std::endl;
        });
        file << "};" << std::endl;
    }
}

void process_library_features() {
    if (std::ofstream file{"library_features.hpp"}; file.is_open()) {
        file << "std::map<std::string, long> library_features{" << std::endl;
        auto features = get_library_features();
        std::for_each(std::begin(features), std::end(features),
        [&file=file](library_feature feature) {
            auto macro = feature.test_macro();
            file << "{\"" << macro << "\", " << std::endl
            << "#ifdef " << macro << std::endl
            << macro << std::endl
            << "#else" << std::endl
            << "0" << std::endl
            << "#endif" << std::endl
            << "}," << std::endl;
        });
        file << "};" << std::endl;
    }
}

int main() {
    process_attributes();
    process_language_features();
    process_library_features();
    return 0;
}
