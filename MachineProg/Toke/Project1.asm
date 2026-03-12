        
        .ORIG x3000
        LD R0, A
        LD R1, B
        
X       AND R2, R2, #0
        ADD R2, R2, R0
        ADD R2, R2, R1
        BRz Done
        ADD R1, R1, #-1
        ADD R2, R2, #1
        BRnzp   X
        
DONE    ST      R1,C
        TRAP    x25
        
A       .BLKW   1
B       .BLKW   1
C       .BLKW   1
        .END