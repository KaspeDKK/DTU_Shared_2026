.ORIG   x3000
    JSR     readS          ; R0 = typed decimal number
    JSR     isPrime        ; R0 = 1 if prime, 0 if not
    JSR     resultS        ; print result
    HALT

;---------------------------------------
; readS
; reads a decimal number with 1 to 5 digits
; validates input format
; returns number in R0
; on invalid input prints error message and halts
;---------------------------------------
readS
    LEA     R0, MSG
    PUTS

    AND     R4, R4, #0     ; R4 = current value
    AND     R6, R6, #0     ; R6 = digit count

readLoop
    GETC
    OUT

    ; check for Enter
    LD      R1, NEWLINE
    NOT     R1, R1
    ADD     R1, R1, #1
    ADD     R1, R0, R1
    BRz     checkDone

    ; if already 5 digits, any extra char is invalid
    ADD     R1, R6, #-5
    BRzp    tooManyDigits

    ; convert ASCII to numeric candidate: char - '0'
    LD      R1, ASCII_ZERO
    NOT     R1, R1
    ADD     R1, R1, #1
    ADD     R2, R0, R1     ; R2 = char - '0'

    ; if negative, char was below '0'
    BRn     invalidInput

    ; if R2 >= 10, char was above '9'
    ADD     R1, R2, #-10
    BRzp    invalidInput

    ; value = value * 10
    ADD     R3, R4, R4     ; 2*value
    ADD     R5, R3, R3     ; 4*value
    ADD     R5, R5, R5     ; 8*value
    ADD     R4, R3, R5     ; 10*value

    ; value = value + digit
    ADD     R4, R4, R2

    ; count accepted digit
    ADD     R6, R6, #1
    BRnzp   readLoop

checkDone
    ; empty input is invalid
    ADD     R1, R6, #0
    BRz     invalidInput

    AND     R0, R0, #0
    ADD     R0, R0, R4
    RET

tooManyDigits
    LEA     R0, errTooLong
    PUTS
    HALT

invalidInput
    LEA     R0, errFormat
    PUTS
    HALT

;---------------------------------------
; isPrime
; inputs -> R0 = number
; outputs -> R0 = 1 if prime, 0 if not
;---------------------------------------
isPrime
    ADD     R1, R0, #-1
    BRnz    isNotPrime

    ADD     R1, R0, #-2
    BRz     isPrimeTrue

    AND     R2, R2, #0
    ADD     R2, R2, #2     ; divisor = 2

divisorLoop
    NOT     R3, R2
    ADD     R3, R3, #1
    ADD     R3, R0, R3     ; n - divisor
    BRz     isPrimeTrue

    ADD     R4, R0, #0     ; working copy of n

subtractionLoop
    NOT     R5, R2
    ADD     R5, R5, #1
    ADD     R4, R4, R5     ; R4 = R4 - divisor
    BRz     isNotPrime
    BRp     subtractionLoop

    ADD     R2, R2, #1
    BRnzp   divisorLoop

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
MSG             .STRINGZ "Input a decimal number (1 to 5 digits): "
stringPrime     .STRINGZ "\nThe number is prime"
stringNotPrime  .STRINGZ "\nThe number is not prime"
errFormat       .STRINGZ "\nError: input must contain digits only (0-9)"
errTooLong      .STRINGZ "\nError: input can contain at most 5 digits"
ASCII_ZERO      .FILL   x0030
NEWLINE         .FILL   x000A

.END