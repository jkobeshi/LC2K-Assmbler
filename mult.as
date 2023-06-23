        lw      0       6       mcand
        lw      0       7       mplier
        lw      0       2       one
        lw      0       3       max
        lw      0       0       mcand
        nor     7       7       4
        nor     2       2       5
        nor     5       4       4
        beq     4       2       addAns
start   add     0       0       0
        add     2       2       2
        nor     7       7       4
        nor     2       2       5
        nor     5       4       4
        beq     4       2       addAns
        beq     2       3       done
        beq     0       0       start
addAns  add     1       0       1
        beq     2       3       done 
        beq     0       0       start
done    halt                                end of program
mcand   .fill   1103
mplier  .fill   7043                
max     .fill   65536
one     .fill   1