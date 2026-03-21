.ORIG x3000 


JSR readS




readS   ;Gemmer det som er i registrene.
        ST R0, SaveR0   ;input area
        ST R1, SaveR1   ;loop-counter
        ST R2, SaveR2   ;loop-max
        ST R3, SaveR3   ;saved val a
        ST R4, SaveR4   ;loop-condition calculation
        ST R7, SaveR7
        
        ;initialize:
        AND R1, R0, #0      ;r1 counter = 0
        AND R2, R0, #0      ;r2 loop condition
        ADD R2, R2, #-9
        LD R3 Asci          ;Fjerner ascii 
        
        ;Output af besked:
        LEA R0, String 
        PUTS
        
        ;Input af første tegn.
        IN
        
        ADD R0, R0, R3      ;r0 "clean" tal
        
        AND R3, R3, #0      ;
        ADD R3, R0, #0      ;gemmer input-tallet i r3
        BRnzp loopA

loopA   ADD R0, R0, R3      
        ADD R1, R1, #1
        ADD R4, R2, R1 
        BRnp loopA
        BRz nextIn
        
nextIn  ADD R1, R0, #0      ;Gemmer tal fra loopA i r1
        In                  ;tal b i r0
        LD R3 Asci          ;fjerner aski værdi fra r0
        ADD R0, R0, R3
        
        ADD R0, R1, R0      ;a+b
        
        
        ;Reloading før return
        LD R1, SaveR1
        LD R2, SaveR2
        LD R3, SaveR3
        LD R4, SaveR4
        LD R7, SaveR7   ; Gør R7 klar til at finde tilbage til HALT
        RET             ; Gå tilbage til JSR-linjen

Asci        .FILL #-48
String      .STRINGZ "Input a 2 digit decimal number: "
SaveR0
SaveR1      .BLKW 1
SaveR2      .BLKW 1
SaveR3      .BLKW 1
SaveR4      .BLKW 1
SaveR7      .BLKW 1

.END