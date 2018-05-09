;;===============================
;;Name: Jesse Chen
;;===============================

.orig x3000

LD R0, A        ;Load A into R0
LD R1, B        ;Load B into R1

ADD R2, R0, R1	;A+B into R2
STI R2, DATA1   ;Put A+B in x5001

AND R3, R0, R1	;A&B into R3
STI R3, DATA2	;Put A&B in x5002

NOT R4, R0		;Being two's complement
ADD R4, R4, 1   ;Finish two's complement for -A
STI R4, DATA3	;Put -A in x5003

ADD R5, R0, R0	;2 * A
ADD R6, R1, R1	;2 * B
ADD R6, R6, R1	;3 * B
ADD R6, R5, R6	;2 * A + 3 * B
STI R6, DATA4	;Put 2 * A + 3 * B in x5004

HALT

A       .fill 157
B       .fill 257
DATA1	.fill x5001
DATA2	.fill x5002
DATA3	.fill x5003
DATA4	.fill x5004

.end