.text
.code 16
.global handler_reset, vectors_start, vectors_end

handler_reset:
    LDR r1, =_stack_        /* load r1(sp) with start of stack addr */
    BL copy_vectors
    BL main
