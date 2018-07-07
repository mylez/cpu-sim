.data
page_table:
    .byte   0x00
    .byte   0x00
    .byte   0x00
    .byte   0x00
incrementor:
    .byte   0x00

.text
main:
    vset    page_table
    tset    0x10                ; set preemption timer to 16 instructions
    eset    handle_exception    ; designate code for exception handling
    cset    0x80                ; switch to user mode
user_loop:
    add     incrementor
    addi    1
    stor    incrementor
    clac
    beqz    user_loop
handle_exception:
    halt