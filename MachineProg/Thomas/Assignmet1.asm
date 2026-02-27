.ORIG x3000

;Load Values
;   First AND it to make it = 0
;   Then add the EVEN numbers.
;
        AND R0, R0, #0
        AND R1, R1, #0
        AND R3, R3, #0
        
        ADD R0, R0, #10 ;; R0 <- A-value (must be lower than B)
        ADD R1, R1, #6  ;; R1 <- B-value 
        LD R0, A
        LD R1, B
        
X       
        ;
        
        ADD R2, R2, R1
        ;
        ADD R1, R1, #-1
        ;

        BRnzp X

DONE    ST R1, C
        TRAP x25
A       .BLKW 1
B       .BLKW 1
C       .BLKW 1

.END