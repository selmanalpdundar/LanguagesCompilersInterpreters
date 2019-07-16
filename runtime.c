/**
 * @file runtime.c
 * @author Selman ALPDÜNDAR (s.alpdundar@studenti.unipi.it)
 * @brief 
 * @version 0.1
 * @date 2019-05-16
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "stdint.h"
#include "stdio.h"

/**
 * @brief 
 * It called by llvm to print a literal
 * @param x is a literal
 */
void print_i32(int32_t x) {
  printf("%d\n", x);
}



/**
 * @brief 
 * It called by llvm to print a boolean literal
 * @param x is a bool literal
 */
void print_i1(int x) {
  if (x) {
    printf("true\n");
  } else {
    printf("false\n");
  }
}









