#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <functional>

using namespace std::string_literals;

class Type
{
public:
    Type(std::string name) : name{name}
    {
        if (is_header_errornous())
        {
            throw std::logic_error("The header test.hpp contains errors");
        }
        m_DefaultConstructible = check_DefaultConstructible();
        m_MoveConstructible = check_MoveConstructible();
        m_CopyConstructible = check_CopyConstructible();
        m_MoveAssignable = check_MoveAssignable();
        m_CopyAssignable = check_CopyAssignable();
        m_Destructible = check_Destructible();
        m_lvalueSwappable = check_lvalueSwappable();
        m_LegacyIterator = check_LegacyIterator();
        m_EqualityComparable = check_EqualityComparable();
        m_LegacyInputIterator = check_LegacyInputIterator();
    }

    bool is_MoveConstructible()
    {
        return m_MoveConstructible;
    }
    bool is_DefaultConstructible()
    {
        return m_DefaultConstructible;
    }
    bool is_CopyConstructible()
    {
        return m_CopyConstructible;
    }
    bool is_MoveAssignable() {
        return m_MoveAssignable;
    }
    bool is_CopyAssignable() {
        return m_CopyAssignable;
    }
    bool is_Destructible() {
        return m_Destructible;
    }
    bool is_lvalueSwappable() {
        return m_lvalueSwappable;
    }
    bool is_LegacyIterator() {
        return m_LegacyIterator;
    }
    bool is_EqualityComparable() {
        return m_EqualityComparable;
    }
    bool is_LegacyInputIterator() {
        return m_LegacyInputIterator;
    }

private:
    bool is_header_errornous()
    {
        return false == requires_valid([](std::ofstream &file) {

               });
    }
    bool check_MoveConstructible()
    {
        return requires_valid([type = name, func = __func__](std::ofstream &file)
                              {
            file << "#include <utility>" << std::endl;
            file << "void " << func << "(" << type << "&& rv){" << std::endl;
            file << "    " << type << " u = std::move(rv);" << std::endl;
            file << "    " << type << "(std::move(rv));}" << std::endl; });
    }
    bool check_DefaultConstructible()
    {
        return requires_valid([type = name](std::ofstream &file)
                              {
    file << "void check_DefaultConstructible(){" << std::endl;
    file << "    " << type << " u;" << std::endl;
    file << "    " << type << " u1{};" << std::endl;
    file << "    " << type << "();" << std::endl;
    file << "    " << type << "{};" << std::endl;
    file << "}" << std::endl; });
    }
    bool check_CopyConstructible()
    {
        return check_MoveConstructible() && requires_valid([type = name, func = __func__](std::ofstream &file)
                                                           {
            file << "void " << func << "(" << type << " v) {" << std::endl;
            file << type << " u = v;" << std::endl;
            file << "(" << type << "(v));" << std::endl;
            file << "}" << std::endl; });
    }
    bool check_MoveAssignable()
    {
        return requires_valid([type = name, func = __func__](std::ofstream &file)
                              {
            file << "#include <utility>" << std::endl;
            file << "void " << func << "(" << type << "& t, "<< type << "&& rv){" << std::endl;
            file << "    t = std::move(rv);" << std::endl;
            file << "}" << std::endl; });
    }
    bool check_CopyAssignable()
    {
        return check_MoveAssignable() &&
               requires_valid(
                   "void "s + __func__ + "("s + name + "& t, "s + name + "& v) {\n"s +
                   "    t = v;\n"s +
                   "}\n"s);
    }
    bool check_Destructible()
    {
        return requires_valid(
            "void "s + __func__ + "("s + name + "& u) {\n"s +
            "    u.~"s + name + "();"s
            "}\n"s);
    }
    bool check_lvalueSwappable()
    {
        return requires_valid(
                        "#include <utility>\n"s +
            "void "s + __func__ + "("s + name + "& u, "s + name + "& t) {\n"s +
            "using std::swap;\n"s +
            "swap(u, t);\n"s +
            "}\n"s);
    }
    bool check_LegacyIterator()
    {
        return check_CopyConstructible() &&
               check_CopyAssignable() &&
               check_Destructible() &&
               check_lvalueSwappable() &&
               requires_valid(
                "#include <iterator>\n"s +
                "using value_type = std::iterator_traits<"s + name + ">::value_type;\n"s +
                "using difference_type = std::iterator_traits<"s + name + ">::difference_type;\n"s +
                "using reference = std::iterator_traits<"s + name + ">::reference;\n"s +
                "using pointer = std::iterator_traits<"s + name + ">::pointer;\n"s + 
                "using iterator_category = std::iterator_traits<"s + name + ">::iterator_category;\n"s +
                "void "s + __func__ + "("s + name + "& r){\n"s +
                "    *r;\n"s +
                "    ++r;\n"s +
                "}\n"s
               );
    }
    bool check_EqualityComparable() {
        return requires_valid(
            "void "s + __func__ + "("s + name + " a, "s + name + " b, "s + name + " c){\n"s +
            "    bool tmp = (a == b);\n" +
            "}\n"
        );
    }
    bool check_LegacyInputIterator() {
        return check_LegacyIterator() &&
        check_EqualityComparable() &&
        requires_valid(
            "#include <iterator>\n"s +
            "using reference = std::iterator_traits<"s + name + ">::reference;\n"s +
            "using value_type = std::iterator_traits<"s + name + ">::value_type;\n"s +
            "void "s + __func__ + "("s + name + " i, "s + name + " j, "s + name + " r){\n"s +
            "    bool tmp = (i != j);\n"s +
            "    *i;\n"s +
            "    ++r;\n"s +
            "    (void)r++;\n"s +
            "    *r++;\n"s +
            "}\n"
        );
    }
    bool requires_valid(const std::function<void(std::ofstream &)> &func)
    {
        std::ofstream file{"test.cpp"};
        file << "#include \"test.hpp\"" << std::endl;
        func(file);
        file.close();
        int ret = std::system("cl /c test.cpp");
        return ret == 0;
    }
    bool requires_valid(std::string str)
    {
        std::ofstream file{"test.cpp"};
        file << "#include \"test.hpp\"" << std::endl;
        file << str;
        file.close();
        int ret = std::system("cl /std:c++20 /c test.cpp");
        return ret == 0;
    }

    std::string name;
    bool m_DefaultConstructible;
    bool m_MoveConstructible;
    bool m_CopyConstructible;
    bool m_MoveAssignable;
    bool m_CopyAssignable;
    bool m_Destructible;
    bool m_lvalueSwappable;
    bool m_LegacyIterator;
    bool m_EqualityComparable;
    bool m_LegacyInputIterator;
};

int main()
{
    std::cout << "Which type do you want to check for DefaultConstructible: ";
    std::string type_name;
    std::cin >> type_name;
    try
    {
        Type type{type_name};
        std::cout << type_name << ":" << std::endl;
        std::cout << "    DefaultConstructible    :    " << type.is_DefaultConstructible() << std::endl;
        std::cout << "    MoveConstructible       :    " << type.is_MoveConstructible() << std::endl;
        std::cout << "    CopyConstructible       :    " << type.is_CopyConstructible() << std::endl;
        std::cout << "    MoveAssignable          :    " << type.is_MoveAssignable() << std::endl;
        std::cout << "    CopyAssignable          :    " << type.is_CopyAssignable() << std::endl;
        std::cout << "    Destructible            :    " << type.is_Destructible() << std::endl;
        std::cout << "    lvalueSwappable         :    " << type.is_lvalueSwappable() << std::endl;
        std::cout << "    LegacyIterator          :    " << type.is_LegacyIterator() << std::endl;
        std::cout << "    EqualityComparable      :    " << type.is_EqualityComparable() << std::endl;
        std::cout << "    LegacyInputIterator     :    " << type.is_LegacyInputIterator() << std::endl;
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}