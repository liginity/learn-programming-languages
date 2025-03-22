#include <iostream>

class Item {
   public:
    Item() {
        // default ctor
        std::cout << "Item()\n";
    }

    Item(int num) : num_(num) {
        // coverting ctor
        std::cout << "Item(int)\n";
    }

    Item(const Item &) = delete;

    // Item(const Item &) {
    //     // copy ctor
    //     std::cout << "Item(const Item&)\n";
    // }

    Item(const Item &&) {
        // copy ctor
        std::cout << "Item(const Item&&)\n";
    }

    Item &operator=(const Item &) {
        // copy assign
        std::cout << "operator=(const Item&)\n";
        return *this;
    }

   private:
    int num_;
};

int main() {
    Item item1;
    Item item2(10);
    // error in c++11
    // NOTE this is copy-init, and I think here the rule is:
    //     int would be converted to Item, as a prvalue temporary, and the
    //     copy-ctor or move-ctor theoretically would be used to init the
    //     variable. copy-ctor (or move-ctor) is not actually called, but
    //     copy-ctor (or move-ctor) should be accessible.
    // ok in c++17
    Item item3 = 11;
}
