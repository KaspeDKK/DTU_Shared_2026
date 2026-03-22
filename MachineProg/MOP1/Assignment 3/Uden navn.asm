.ORIG   x3000



;---------
;funktion der tjekker om et tal er et primtal 
;
;---------

readS

    AND R1,R1,#0    ;nulstiller registre
    AND R2,R2,#0 
    AND R3,R3,#0
    
    ADD R2,R2,#1    ;divisor 
    ADD R1,R0,R1    ;tilføjer input - R0 til R1 
    
    ADD R1,R1,#-1   ;R0 = 1?
    BRz     isNotPrime
    
    ADD R1,R1,#-1   ;R0 = 2? 
    BRz     isPrime
    BRp     divisorLoop

divisorLoop 
    AND R1,R1,#0 ;R1 = R0 (input)
    ADD R1,R0,R1
    
    ADD R2,R2,#1    ;Increment divisor 
    NOT R2,R2       ;-divisor
    ADD R2,R2,#1
    
    ADD R1,R1,R2
    BRz     isPrime
    BRp     subtractionLoop

    
subtractionLoop
    
    ADD R1,R1,R2
    BRp subtractionLoop
    BRz isNotPrime
    BRn divisorLoop
    
    
    
    isPrime
        AND R0,R0,#0
        ADD R0,R0,#1
    JSR endReadS
    
    isNotPrime
        AND R0,R0,#0
    JSR endReadS
    
    
    endReadS
    HALT
    .end ; Skiftes ud med return
  
    

    