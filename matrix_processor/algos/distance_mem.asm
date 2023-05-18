.info
    set_entry start
    debug off
    nb_reg 9
    size auto


.code


start:
    LOAD 0 0                ; mat = ram[0]
    SET_FILL 8 0            ; empty = 0
    SAVE 8 0                ; ram[0] = empty
    SET_VAL 0 1 1 1         ; mat.data[1][1] = 1
    SET_ID 1                ; borne_inf = Matrix.IDENTITY(mat.x)
    SET_ID 2                ; borne_inf_counter = 1
    MOVE 3 0                ; times = mat
    SET_ID 4                ; times_counter = 1
    SET_FILL 5 0            ; times_index = 0
    JMP $loop

loop:                       ; while True:
    MUL 1 3 6               ;   k(reg6) = borne_inf * times
    MOVE 7 6                ;   
    SET_VAL 7 0 1 0         ;   
    SUB 6 7 7               ;   
    JMP_NULL 7 $if          ;   if k.data[0][1] == 0: ($if)
    JMP $else               ;   else: ($else)

if:
    LOAD_AT 7 5             ; t = ram[times_index]
    JMP_NULL 7 $prod        ; if t == 0: ($prod)
    JMP $load_val

load_val:
    PRINT_STR MEMOISATION_!
    LOAD_AT 3 5             ; times = ram[times_index]
    JMP $continue

prod:
    MUL 3 3 3               ; times *= times
    SAVE_AT 3 5             ; ram[times_index] = times
    JMP $continue

continue:
    ADD 2 4 2               ; borne_inf_counter += times_counter
    PRINT_STR ---Adding---
    ADD 4 4 4               ; times_counter += times_counter
    MOVE 1 6                ; borne_inf = k
    SET_ID 7
    ADD 5 7 5               ; times_index += 1
    JMP $loop

else:
    SUB 3 0 7
    PRINT_STR ---Resetting...---
    JMP_NULL 7 $end         ; if times == mat: ($end)
    MOVE 3 0                ; times = mat
    SET_ID 4                ; times_counter = 1
    SET_FILL 5 0            ; times_index = 0
    JMP $loop               ; end while


end:
    PRINT_STR Ending
    exit_val 2              ; return borne_inf_counter

.ram
    arg 0