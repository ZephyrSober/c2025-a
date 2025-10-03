cmake_minimum_required(VERSION 4.0)
project(c2025_challenge)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(p01_running_letter level1/p01_running_letter/main.c
        level1/p01_running_letter/utils.c
        level1/p01_running_letter/utils.h)

add_executable(p02_is_prime  level1/p02_is_prime/main.c)

add_executable(p03_all_primes  level1/p03_all_primes/main.c)

add_executable(p04_goldbach  level1/p04_goldbach/main.c)

add_executable(p05_encrypt_decrypt  level1/p05_encrypt_decrypt/main.c)

add_executable(p06_hanoi  level1/p06_hanoi/main.c)

add_executable(p07_maze  level1/p07_maze/main.c)

add_executable(p08_push_boxes  level1/p08_push_boxes/main.c
        level1/p08_push_boxes/utils.c
        level1/p08_push_boxes/utils.h)

add_executable(p09_linked_list  level1/p09_linked_list/main.c)

add_executable(p10_warehouse  level1/p10_warehouse/main.c
        level1/p10_warehouse/utils.c
        level1/p10_warehouse/utils.h)
add_executable(queue extend_level/queue.c)
add_executable(stack extend_level/stack.c
)
add_executable(antipoland extend_level/antipoland.c
)
add_executable(MCTS Assignment/MCTS.c
        Assignment/strategy.c
        Assignment/strategy.h
        Assignment/List.c
        Assignment/List.h
        Assignment/utils.c
        Assignment/utils.h
        Assignment/test.c
        Assignment/test.h)
add_executable(mxts_test Assignment/test.c)
