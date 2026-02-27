        .ORIG x3000
        LD R0, A
        LD R1, B
x       ------------------ (a)
        ------------------ (b)
        ADD R2,R2,R1
        ------------------ (c)
        ADD R1,R1,#-1
        ------------------ (d)
        BRnzp   X
DONE    ST      R1,C
        TRAP    x25
A       .BLKW   1
B       .BLKW   1
C       .BLKW   1
        .END