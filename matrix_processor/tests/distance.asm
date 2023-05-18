.info
    set_entry start
    debug off
    nb_reg 7
    size auto


.code


start:
    LOAD 0 0
    SET_VAL 0 1 1 1         ; mat.data[1][1] = 1
    SET_ID 1                ; borne_inf = Matrix.IDENTITY(mat.x)
    SET_ID 2                ; borne_inf_counter = 1
    MOVE 3 0                ; times = mat
    SET_ID 4                ; times_counter = 1
    JMP $loop

loop:                       ; while True:
    MUL 1 3 5               ;   k = borne_inf * times
    MOVE 6 5                ;   
    SET_VAL 6 0 1 0         ;
    SUB 5 6 6               ;
    JMP_NULL 6 $if          ;   if k.data[0][1] == 0: ($if)
    JMP $else               ;   else: ($else)

if:
    MUL 3 3 3               ; times *= times # read from table ?
    ADD 2 4 2               ; borne_inf_counter += times_counter
    PRINT_STR ---Adding---
    ADD 4 4 4               ; times_counter += times_counter
    MOVE 1 5                ; borne_inf = k
    JMP $loop

else:
    SUB 3 0 6
    PRINT_STR ---Resetting...---
    JMP_NULL 6 $end         ; if times == mat: ($end)
    MOVE 3 0                ; times = mat
    SET_ID 4                ; times_counter = 1
    JMP $loop               ; end while


end:
    PRINT_STR Ending
    exit_val 2              ; return borne_inf_counter


.ram
    arg 0