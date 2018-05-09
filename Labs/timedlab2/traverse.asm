;;==============================================
;; NAME: Jesse Chen
;;==============================================


.orig x3000

        LD R0, MAX_VAL  ;Load MAX_VAL into R0
        LD R1, ODD_CNT  ;Load ODD_CNT into R1
        LD R2, LENGTH   ;Load LENGTH (i) into R2
        LEA R6, ARRAY	;Load ARRAY starting index into R6

        LD R3, ARRAY	;Load data from ARRAY[i] into R3

        ;Test loop condition (ARRAY[i] != -1)
TEST    LDR R3, R6, 0	;Load new ARRAY[i]
		ADD R3, R3, 1   ;ARRAY[i]++
        BRz BREAK       ;If ARRAY[i] == -1 -> BREAK
        ADD R3, R3, -1	;Reset R3 to ARRAY[i]

        ;While loop body
        ;First if conditions
        NOT R4, R0		;Two's complement MAX_VAL in R4
        ADD R4, R4, 1	;MAX_VAL = -MAX_VAL
        ADD R4, R3, R4	;ARRAY[i] - MAX_VAL
        BRnz ELSE1		;If (ARRAY[i] > MAX_VAL) -> NEXT

        ;First if body
        AND R0, R0, 0	;Reset R0
        ADD R0, R0, R3	;Save new MAX_VAL in R0

        ;Second if conditions
ELSE1	AND R5, R3, 1	;R5 = R3 & 1
		ADD R5, R5, -1	;R5--
		BRnp ELSE2		;If (ARRAY[i] & 1 == 1)

		;Second if body
		ADD R1, R1, 1	;ODD_CNT++

ELSE2	ADD R2, R2, 1	;LENGTH++
		ADD R6, R6, 1	;Increment array index
        BR TEST 		;End of while loop body, loop back to beginning

BREAK   ST R0, MAX_VAL	;Store R0 in MAX_VAL
		ST R1, ODD_CNT	;Store R1 in ODD_CNT
		ST R2, LENGTH 	;Store R2 in LENGTH

        HALT

MAX_VAL .fill 0			; Store the maximum value in the array at this label
ODD_CNT	.fill 0			; Store the total number of odd entries in the array at this label
LENGTH	.fill 0			; Store the length of the array at this label

ARRAY	.fill 1			; The first element of the array
	.fill 3
	.fill 7
	.fill 4
	.fill 2
	.fill 9
	.fill 3
	.fill 4			; The last element of the array
	.fill -1		; -1 will follow the last element of the array	


.end
