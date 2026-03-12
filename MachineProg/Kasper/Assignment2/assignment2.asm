.ORIG x3000

        LEA R0, MSG
        PUTS

        ; first digit
        GETC
        OUT
        ADD R5, R0, #0       ; save character from R0

        LD R6, ASCII0
        NOT R6, R6
        ADD R6, R6, #1       ; R6 = -'0'
        ADD R1, R5, R6       ; R1 = first digit

        ; multiply first digit by 10
        ADD R2, R1, R1       ; 2x
        ADD R3, R2, R2       ; 4x
        ADD R3, R3, R3       ; 8x
        ADD R2, R2, R3       ; 2x + 8x = 10x

        ; second digit
        GETC
        OUT
        ADD R5, R0, #0       ; save character from R0

        LD R6, ASCII0
        NOT R6, R6
        ADD R6, R6, #1       ; R6 = -'0'
        ADD R4, R5, R6       ; R4 = second digit

        ; result = 10*first + second
        ADD R0, R2, R4       ; final answer must be in R0
        TRAP x25

MSG     .STRINGZ "Input a 2 digit decimal number:"
ASCII0  .FILL x0030
.END