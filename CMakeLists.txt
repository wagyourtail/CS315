cmake_minimum_required(VERSION 3.23)
set(CMAKE_C_STANDARD 23)

project(CS315_HW0 C)
project(CS315_HW1 C)
project(CS315_HW2 C)
project(CS315_HW3 C)
project(CS315_HW3_extra C)
project(CS315_HW4 C)
project(CS315_DS C)

add_executable(CS315_DS
        DataStructures/BinaryTree.c
        DataStructures/Heap.c
        DataStructures/HuffmanTree.c
        DataStructures/LinkedList.c
        DataStructures/Queue.c
        DataStructures/Stack.c
)


add_executable(CS315_HW0 HW0/grayw7_hw0.c)
add_executable(CS315_HW1 HW1/grayw7_hw1.c)
add_executable(CS315_HW2 HW2/grayw7_hw2.c)
add_executable(CS315_HW3 HW3/grayw7_hw3.c)
add_executable(CS315_HW3_extra HW3/grayw7_hw3_post_order.c)
add_executable(CS315_HW4 HW4/grayw7_hw4.c)