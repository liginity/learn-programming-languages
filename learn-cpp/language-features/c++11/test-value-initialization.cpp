#include <iostream>

struct Item1 {
    int mem1_;
    int mem2_;
};

struct Item2 {
    int mem1_;
    int mem2_;

    Item2() : mem1_() {}
};

void f1(int i);

int main() {
    constexpr int N = 4;
    for (int i = 0; i < N; ++i) {
        // std::cout << "#" << i << "\n";
        f1(i);
    }
}

void f1(int i) {
    Item1 item1_obj1;
    Item2 item2_obj1;

    std::cout << "Item1 (default init): mem1 = " << item1_obj1.mem1_
              << ", mem2 = " << item1_obj1.mem2_ << "\n";
    std::cout << "Item2 (default init): mem1 = " << item2_obj1.mem1_
              << ", mem2 = " << item2_obj1.mem2_ << "\n";
    std::cout << "&item1_obj1 = " << &item1_obj1
              << ", &item2_obj1 = " << &item2_obj1 << "\n";

    item1_obj1.mem1_ = 10 + i;
    item1_obj1.mem2_ = 100 + i;
    item2_obj1.mem1_ = 10 + i;
    item2_obj1.mem2_ = 100 + i;


    Item1 item1_obj2{};
    Item2 item2_obj2{};

    std::cout << "Item1 (value init)  : mem1 = " << item1_obj2.mem1_
              << ", mem2 = " << item1_obj2.mem2_ << "\n";
    std::cout << "Item2 (value init)  : mem1 = " << item2_obj2.mem1_
              << ", mem2 = " << item2_obj2.mem2_ << "\n";

    item1_obj2.mem1_ = 10 + i;
    item1_obj2.mem2_ = 100 + i;
    item2_obj2.mem1_ = 10 + i;
    item2_obj2.mem2_ = 100 + i;

    std::cout << "#" << i << "\n";
}

/* one result

Item1 (default init): mem1 = 0, mem2 = 0
Item2 (default init): mem1 = 0, mem2 = 0
&item1_obj1 = 0x7fff34968d98, &item2_obj1 = 0x7fff34968d90
Item1 (value init)  : mem1 = 0, mem2 = 0
Item2 (value init)  : mem1 = 0, mem2 = 0
#0
Item1 (default init): mem1 = 10, mem2 = 100
Item2 (default init): mem1 = 0, mem2 = 100
&item1_obj1 = 0x7fff34968d98, &item2_obj1 = 0x7fff34968d90
Item1 (value init)  : mem1 = 0, mem2 = 0
Item2 (value init)  : mem1 = 0, mem2 = 100
#1
 */
