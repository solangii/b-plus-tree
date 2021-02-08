#include <iostream>
#include "BPlusTree.h"

void print_bool(bool cond) {
    if (cond) {
        std::cout << "True\n";
    } else {
        std::cout << "False\n";
    }
}

int main() {
    BPlusTree<int> bpt(15);

    for(int i=1; i<136; i++){
        bpt.insert(i*13+87);
        std::cout<<i*13+87<<' ';
        bpt.insert(i*99-28);
        std::cout<<i*99-28<<' ';

    }

    std::cout<<std::endl;
    bpt.bpt_print();
//    std::cout<<std::endl<<std::endl;


//    int* result_array = new int[8];
//    int result = bpt.range_search(150, 240, result_array, 8);
//
//    std::cout << result << '\n'; // 8
//
//    for (int i = 0; i < result; i++) {
//        std::cout << result_array[i] << ' ';
//    }
//    std::cout << '\n'; // 5, 6, 10*/

    bpt.remove(152);
    bpt.remove(269);
    bpt.remove(256);


    std::cout<<"ㄴㄴㄴ"<<std::endl;
    bpt.remove(467);
    bpt.bpt_print();

//    bpt.remove(217);

//
//    std::cout<<std::endl;
//    bpt.bpt_print();
    std::cout<<std::endl<<std::endl;
    bpt.remove(764);
    bpt.remove(863);

    std::cout<<std::endl;
//    bpt.getroot();
    bpt.bpt_print();

    bpt.remove(71);
    bpt.remove(100);
    bpt.remove(139);

    std::cout<<std::endl;
    bpt.bpt_print();

    bpt.remove(368);


    std::cout<<std::endl;
    bpt.bpt_print();




//
//
//
//
//    BPlusTree<int> bpt(3);
//    bpt.insert(1);
//    bpt.insert(2);
//    bpt.insert(3);
//    bpt.insert(4);
//    bpt.insert(5);
//
//    print_bool(bpt.search(2)); // True
//    print_bool(bpt.search(42)); // False
//
//
//    bpt.remove(3);
//
//    bpt.bpt_print();
//
//    print_bool(bpt.search(3)); // False
//
//    bpt.insert(6);
//    bpt.insert(10);
//    std::cout << '\n';
//    bpt.bpt_print();
//
//    std::cout << '\n';
//
//    int* result_array = new int[3];
//    int result = bpt.range_search(3, 7, result_array, 3);
//
//    std::cout << result << '\n'; // 3
//    for (int i = 0; i < result; i++) {
//        std::cout << result_array[i] << ' ';
//    }
//    std::cout << '\n'; // 5, 6, 10
}