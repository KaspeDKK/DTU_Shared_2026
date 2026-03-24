
.ORIG x3000

MAIN
    JSR readS
    JSR isPrime
    JSR resultS
    BRnzp MAIN

;---------------------------------------

readS

LEA     R0, MSG     ; load address of string into R0
TRAP    x22         ; PUTS: print string from R0

TRAP    x23         ;Tager et argument som ASCII værdi    
ADD     R1, R0, #0  ;Kopiere argument
TRAP    x23         ;
ADD     R2, R0, #0

; Konvertering af ASCII til numerisk
LD      R3, ASCII_ZERO
NOT     R3, R3
ADD     R3, R3, #1      ; R3 = -'0'

ADD     R1, R1, R3      ; R1 = Vi laver om til numerisk værdi
ADD     R2, R2, R3      ; R2 = Vi laver om til numerisk værdi

; 10*R1 + R2 
ADD     R4, R1, #0      ; R4 = første gang (ud af 10)
AND     R5, R5, #0
ADD     R5, R5, #9      ; counter = 9 (Vi har allerede x1)

LOOP
ADD     R1, R1, R4      ; gange 10*R1
ADD     R5, R5, #-1     ; en mindre
BRp     LOOP            ; R5 > 0

AND     R0, R0, #0
ADD     R0, R1, R2 
RET


;MSG .STRUNG
MSG     .STRINGZ "Input a 2 digit decimal number"
ASCII_ZERO  .FILL   x0030   ; '0'

        

;----
;Funktion: isPrime
;Hvis tallet er et primtal, skal tallet returneres, ellers skal nul returneres. 
;---


isPrime

    AND R1,R1,#0    ;nulstiller registre
    AND R2,R2,#0 
    AND R3,R3,#0
    
    ADD R2,R2,#-1    ;divisor 
    
    
    ADD R1,R0,R1    ;tilføjer input - R0 til R1 
    
    ADD R1,R1,#-1   ;R0 = 1?
    BRz     isNotPrime
    
    ADD R1,R1,#-1   ;R0 = 2? 
    BRz     isPrimeTrue
    BRp     divisorLoop

divisorLoop 
    AND R1,R1,#0 ;R1 = R0 (input)
    ADD R1,R0,R1
    
    ADD R2,R2,#-1    ;Increment divisor 
    
    ADD R1,R1,R2    ;Divisor >= n? 
    BRz     isPrimeTrue
    BRp     subtractionLoop

    
subtractionLoop
    
    ADD R1,R1,R2 ;Input - divisor
    BRp subtractionLoop
    BRz isNotPrime
    BRn divisorLoop
    
    isPrimeTrue
        AND R0,R0,#0
        ADD R0,R0,#1
        RET

    isNotPrime
        AND R0,R0,#0
        RET

;----
;Funktion: resultS
;Hvis tallet i R0 ≠ 0 : Output prime besked, hvis det er 0: Output ikke prime besked.
;---

resultS ADD R0, R0, #0
        BRp prime
        BRz noPrime


prime   LEA R0, stringPrime
        BRnzp end

noPrime LEA R0, stringNotPrime
        BRnzp end

end     PUTS
        RET     ;return 

stringPrime     .STRINGZ "\nThe number is prime\n"
stringNotPrime  .STRINGZ "\nThe number is not prime\n"

.END