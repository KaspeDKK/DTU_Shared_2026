
.ORIG   x3000
    JSR     readS      ; R0 now contains the number

    JSR     isPrime    ; Returns R0=1 if prime, 0 if not
    JSR     resultS    ; Uses R0 from isPrime
    HALT

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


;----
;Funktion: resultS
;Hvis tallet i R0 ≠ 0 : Output prime besked, hvis det er 0: Output ikke prime besked.
;---

resultS ADD R0, R0, #0
        BRnp prime
        BRz noPrime


prime   LEA R0, stringPrime
        BRnzp end

noPrime LEA R0, stringNotPrime
        BRnzp end

end     PUTS
        RET     ;return 

stringPrime     .STRINGZ "The number is prime"
stringNotPrime  .STRINGZ "The number is not prime"

.END