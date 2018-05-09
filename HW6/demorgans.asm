;;===============================
;;Name: Jesse Chen
;;===============================

.orig x3000

LD R0, A        ;Load A into R0
LD R1, B        ;Load B into R1
LD R2, ANSWER   ;Load ANSWER into R2
NOT R0, R0      ;A = !A
NOT R1, R1      ;B = !B
AND R2, R0, R1  ;ANSWER = A & B
NOT R2, R2      ;ANSWER = !ANSWER
ST R2, ANSWER	;Store answer in ANSWER
HALT
	
A       .fill 6
B       .fill 11
ANSWER  .fill 0
.end