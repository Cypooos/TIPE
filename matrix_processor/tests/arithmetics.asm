.info
    set_entry start
    debug off
    nb_reg 3
    size 2

.code

start:
    LOAD 0 0
    LOAD 1 1
    ADD 0 1 2
    MUL 0 1 2
    PRINT_STR Done,_result_is 
    PRINT_REG 2
    EXIT

.ram
    arg 0
    arg 1