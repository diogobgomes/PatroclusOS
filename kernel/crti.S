# @file crti.S
# @author Diogo Gomes
# @brief crti function, needed by gcc
# @version 0.1
# @date 2024-01-02

.section .init
.global _init
.type _init, @function
_init:
	push %rbp
	mov %rsp, %rbp
	/* gcc will nicely put the contents of crtbegin.o's .init section here. */

.section .fini
.global _fini
.type _fini, @function
_fini:
	push %rbp
	mov %rsp, %rbp
	/* gcc will nicely put the contents of crtbegin.o's .fini section here. */