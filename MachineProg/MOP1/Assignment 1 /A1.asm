.ORIG x3000

;Load Values
;   First AND it to make it = 0
;   Then add the EVEN numbers.
;
        AND R0, R0, #0
        AND R1, R1, #0
        AND R2, R2, #0

        LD R0, A 
        LD R1, B 

        
X       ADD R0, R0, #1
        ADD R1, R1, #-1
        
        ADD R2, R0, #0 ; gets -A
        NOT R2, R2
        ADD R2, R2, #1
        
        ADD R2, R1, R2 ;B-A

        BRnp x
        BRz DONE

DONE    ST R1, C        ;Result-Value is in both R0 and R1
        TRAP x25
A       .FILL #2  ;<- A-value (must be lower than B)
B       .FILL #18 ;R1 <- B-value 
C       .BLKW 1

.END