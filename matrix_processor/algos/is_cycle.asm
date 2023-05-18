.info
    set_entry start
    debug off
    nb_reg 6
    size auto
.code

start:
    LOAD 0 0                ; charger la matrice d'adjacence
    MOVE 1 0                ; R1 := R0
    MOVE 2 0                ; R2 := R0
    LOAD 3 1                ; charger la matrice n*Id
    MOVE 5 0                ; R5 := R0
    SET_ID 4                ; R4 := Id

loop:
    MUL 1 0 1               ; R1 := R1 * R0
    ADD 1 5 5               ; R5 := R5 + R1
    JMP_NULL 1 $acyclique   ; Si R1 = 0: retourner ACYCLIQUE
    MOVE 2 1                ; R2 := R1 
    SUB 3 4 3               ; R3 := R3 - R4
    JMP_NULL 3 $cyclique    ; Si R3 = 0: retourner CYCLIQUE
    JMP $loop               ; Aller à la ligne 6

cyclique:
    PRINT_STR Le_graphe_est_cyclique.
    ;PRINT_REG 5
    EXIT 1

acyclique:
    PRINT_STR Le_graphe_n'est_pas_cyclique.
    ;PRINT_REG 5
    EXIT 0


.ram
    arg 0
    arg_size 0
    
