        .ORIG   x3000
        ;Save current values of registers
        LD      R0,A
        LD      R1,B
        ;reset values
        AND R1,R0,R0 
        
        ;1) A-B = 0?
        ;   NOT(B) + 1
        
X       AND     R3,R1,R0    ;Comparator. 
        BRp     DONE        
        
        ADD     R2,R2,R1
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
        