;readS
.ORIG   x3000

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

ADD     R1, R1, R2 

;MSG .STRUNG
MSG     .STRINGZ "Input a 2 digit decimal number"
ASCII_ZERO  .FILL   x0030   ; '0'
HALT
.END 