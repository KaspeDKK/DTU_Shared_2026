.ORIG x3000

    LD R0, Test
    JSR isPrime
    HALT

;------------------------------------------------------------------------------------------
;Funktion: isPrime
;Tager et tal i R0, og returnere 1 hvis prime og 0 hvis ikke prime. Return sker også i R0.
;------------------------------------------------------------------------------------------

isPrime     
            ;--- Initialize ---
            ;Gemmer alle værdier i working-registers:
            ST R1, saveR1
            ST R2, saveR2
            ST R3, saveR3
            ST R4, saveR4
            ST R5, saveR5
            ST R7, saveR7
            
            AND R2, R2, #0
            
            LEA R1, primeStart  ;R1 -> Adressen for "arrayet" 
            LDR R2, R1, #0      ;R2 -> Load tal, fra pointeren, med ofset på 0 (første værdi)
            
            ;BRn Return0 ;tror jeg er unødvendig
            BRnzp Sammenlign


Sammenlign  LDR R2, R1, #0      ;R2 -> Load tal, fra pointeren, med ofset på R1+0 (0=første værdi)
            BRn Return0         ;Stopper ved -1
            ADD R1, R1, #1      ;pointeren++
            
            NOT R2, R2          ;2' complement
            ADD R2, R2, #1      ;--||--
            
            ADD R2, R0, R2      ;Sammenligner
            BRn Return0         ;false
            BRz Return1         ;true
            BRp Sammenlign      ;loop again
            
            
            
            
Return1     AND R0, R1, #0
            ADD R0, R0, #1
            BRnzp end


Return0     AND R0, R1, #0
            BRnzp end


end         LD R1, saveR1
            LD R2, saveR2
            LD R3, saveR3
            LD R4, saveR4
            LD R5, saveR5
            LD R7, saveR7   
            RET

Test    .Fill #11       
            
primeStart      .FILL #2
                .FILL #3
                .FILL #5
                .FILL #7
                .FILL #11
                .FILL #13
                .FILL #17
                .FILL #19
                .FILL #23
                .FILL #29
                .FILL #31
                .FILL #37
                .FILL #41
                .FILL #43
                .FILL #47
                .FILL #53
                .FILL #59
                .FILL #61
                .FILL #67
                .FILL #71
                .FILL #73
                .FILL #79
                .FILL #83
                .FILL #89
                .FILL #97
primeEnd        .FILL #-1

saveR1  .BLKW 1
saveR2  .BLKW 1
saveR3  .BLKW 1
saveR4  .BLKW 1
saveR5  .BLKW 1
saveR7  .BLKW 1
.END