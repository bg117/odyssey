/**
 * @file terminal.h
 * @author bg117 (bg117@sooosilly.xyz)
 * @brief Text printing functions
 * @version 0.1
 * @date 2023-06-03
 * 
 * This file contains code for printing text to the terminal.
 */
#ifndef ODYSSEY_TERMINAL_H
#define ODYSSEY_TERMINAL_H

#include "limine.h"

/**
 * @brief Initialize the terminal with the given framebuffer.
 * 
 * @param fb The framebuffer to use.
 */
void terminal_init(struct limine_framebuffer *fb);

/**
 * @brief Print a character to the terminal.
 * 
 * @param c The character to print.
 */
void terminal_print_char(char c);

/**
 * @brief Print a string to the terminal.
 * 
 * @param str The string to print.
 */
void terminal_print_string(const char *str);

/**
 * @brief Print a formatted string to the terminal.
 * 
 * @param fmt The format string.
 * @param ... The arguments to the format string.
 */
void terminal_printf(const char *fmt, ...);

#endif