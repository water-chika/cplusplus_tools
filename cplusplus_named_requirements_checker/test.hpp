// Ite satisfies:
//   MoveConstructible
//   CopyConstructible
//   MoveAssignable
//   CopyAssignable
//   Swappable
//   LegacyIterator
//   EqualityComparable
class Ite
{
    // member class
    enum class value
    {
        ite_0,
        ite_1
    };
public:
    // LegacyIterator requires std::iterator_traits<Ite> has member typedefs value_type, difference_type, reference, pointer, and iterator_category.
    // std::iterator_traits<Ite> could use types such as Ite::value_type so Ite define these type aliases.
    using difference_type = void;
    using value_type = value;
    using pointer = void;
    using reference = value&;
    using iterator_category = void;
    // End member class
public:
    // Special member functions
    Ite(Ite &&ite) = default;      // MoveConstructible
    Ite(const Ite &ite) = default; // CopyConstructible
    Ite() = delete;
    Ite &operator=(Ite &&ite) = default;      // MoveAssignable
    Ite &operator=(const Ite &ite) = default; // CopyAssignable
    ~Ite() = default;
    // End special member functions

    Ite(int i) : v{i > 0 ? value::ite_1 : value::ite_0} {}

    // LegacyIterator requires *ite and ++ite in which ite is a lvalue of type Ite.
    reference operator*() {
        return v;
    }
    Ite &operator++() &
    {
        v = (v == value::ite_0 ? value::ite_1 : value::ite_0);
        return *this;
    }
    Ite operator++(int) &
    {
        Ite tmp = *this;
        ++(*this);
        return tmp;
    }
    // EqualityComparable requires a == b in which a, b is expressions of type Ite or const Ite.
    bool operator==(const Ite &ite) const &
    {
        return v == ite.v;
    }

private:
    value v;
};

// Swappable
void swap(Ite &i, Ite &j)
{
    Ite tmp = i;
    i = j;
    j = tmp;
}
void swap(Ite &&i, Ite &j)
{
    j = i;
}
void swap(Ite &i, Ite &&j)
{
    i = j;
}
void swap(Ite &&i, Ite &&j) {}
