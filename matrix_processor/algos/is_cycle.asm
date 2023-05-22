.info
    set_entry start
    debug off
    nb_reg 4
    size auto
.code

start:
    LOAD 0 0                ; charger la matrice d'adjacence dans R0
    MOVE 1 0                ; R1 := R0
    LOAD 2 1                ; charger la matrice n*Id dans R2
    SET_ID 3                ; R3 := Id

loop:
    MUL 1 0 1               ; R1 := R1 * R0
    JMP_NULL 1 $acyclique   ; Si R1 = 0: retourner ACYCLIQUE
    SUB 2 3 2               ; R2 := R2 - R4
    JMP_NULL 2 $cyclique    ; Si R2 = 0: retourner CYCLIQUE
    JMP $loop               ; Aller à la ligne 6

cyclique:
    PRINT_STR Le_graphe_est_cyclique.
    EXIT 1

acyclique:
    PRINT_STR Le_graphe_n'est_pas_cyclique.
    EXIT 0

.ram
    arg 0
    arg_size 0
    
