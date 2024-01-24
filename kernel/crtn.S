# @file crtn.S
# @author Diogo Gomes
# @brief crtn function, needed by gcc
# @version 0.1
# @date 2024-01-02

.section .init
	/* gcc will nicely put the contents of crtend.o's .init section here. */
	pop %rbp
	ret

.section .fini
	/* gcc will nicely put the contents of crtend.o's .fini section here. */
	pop %rbp
	ret