.ORIG   x3000
MAIN
    JSR     readS          ; R0 = typed decimal number
    JSR     isPrime        ; R0 = 1 if prime, 0 if not
    JSR     resultS        ; print result
    BRnzp   MAIN
    
;---------------------------------------
; readS
; reads a decimal number with up to 5 digits
; user types 1..5 digits and presses Enter
; no zero padding needed
; returns number in R0
;---------------------------------------
readS
    LEA     R0, MSG
    PUTS

    AND     R4, R4, #0     ; R4 = current value = 0
    AND     R6, R6, #0
    ADD     R6, R6, #5     ; R6 = remaining digits allowed

readLoop
    GETC                   ; read one character
    OUT                    ; echo it

    ; check if Enter was pressed
    LD      R1, NEWLINE
    NOT     R1, R1
    ADD     R1, R1, #1
    ADD     R1, R0, R1
    BRz     readDone

    ; convert ASCII digit to numeric value
    LD      R1, ASCII_ZERO
    NOT     R1, R1
    ADD     R1, R1, #1
    ADD     R2, R0, R1     ; R2 = digit

    ; multiply current value by 10
    ADD     R3, R4, R4     ; 2*value
    ADD     R5, R3, R3     ; 4*value
    ADD     R5, R5, R5     ; 8*value
    ADD     R4, R3, R5     ; 10*value

    ; add new digit
    ADD     R4, R4, R2

    ; one fewer digit remaining
    ADD     R6, R6, #-1
    BRp     readLoop       ; keep going if still room for more digits

readDone
    AND     R0, R0, #0
    ADD     R0, R0, R4     ; return value in R0
    RET

;---------------------------------------
; isPrime
; inputs -> R0 = number
; outputs -> R0 = 1 if prime, 0 if not
;---------------------------------------
isPrime
    AND     R1, R1, #0
    AND     R2, R2, #0
    AND     R3, R3, #0

    ADD     R2, R2, #-1    ; divisor

    ADD     R1, R0, R1

    ADD     R1, R1, #-1    ; R0 = 1?
    BRz     isNotPrime

    ADD     R1, R1, #-1    ; R0 = 2?
    BRz     isPrimeTrue
    BRp     divisorLoop

divisorLoop
    AND     R1, R1, #0
    ADD     R1, R0, R1

    ADD     R2, R2, #-1    ; increment divisor

    ADD     R1, R1, R2     ; divisor >= n?
    BRz     isPrimeTrue
    BRp     subtractionLoop

subtractionLoop
    ADD     R1, R1, R2     ; input - divisor
    BRp     subtractionLoop
    BRz     isNotPrime
    BRn     divisorLoop

isPrimeTrue
    AND     R0, R0, #0
    ADD     R0, R0, #1
    RET

isNotPrime
    AND     R0, R0, #0
    RET

;---------------------------------------
; resultS
; prints whether the number is prime
;---------------------------------------
resultS
    ADD     R0, R0, #0
    BRp     primeMsg
    BRz     notPrimeMsg

primeMsg
    LEA     R0, stringPrime
    PUTS
    RET

notPrimeMsg
    LEA     R0, stringNotPrime
    PUTS
    RET

;---------------------------------------
; prints and data
;---------------------------------------
MSG             .STRINGZ "Input a decimal number (up to 5 digits): "
stringPrime     .STRINGZ "\nThe number is prime"
stringNotPrime  .STRINGZ "\nThe number is not prime"
ASCII_ZERO      .FILL   x0030
NEWLINE         .FILL   x000A

.END