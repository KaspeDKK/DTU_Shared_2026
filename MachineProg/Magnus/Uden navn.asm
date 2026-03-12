        .ORIG   x3000
        ;Save current values of registers
        LD      R0,A
        LD      R1,B

            ;1) A-B = 0?
        
X           ;   NOT(B) + 1
        AND     R2,R2,#0
        ADD     R2,R2,R1
        NOT     R2,R2
        ADD     R2,R2,#1
        ADD     R2,R0,R2   ;Comparator.
        BRz     DONE        
        
        ADD     R0,R0,#1
        ADD     R1,R1,#-1
        
        BRnzp   X
DONE    ST      R1,C
        TRAP    x25
A       .BLKW   1
B       .BLKW   1
C       .BLKW   1
        HALT
        .END
        