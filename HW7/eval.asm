; Name: Jesse Chen
;RUN FOR 133. DOING EVAL FOR FIRST RIGHT

; Main
; Do not edit this function!

.orig x3000

		LD R6, STACK		; Initialize the stack

		LEA R0, STRING		; R0 = &str[0]
		ADD R1, R0, 0

SL_LOOP	LDR R2, R1, 0		; \ R1 = strlen(str)
		BRz SL_END			; |
		ADD R1, R1, 1		; |
		BR SL_LOOP			; |
SL_END	NOT R2, R0			; |
		ADD R2, R2, 1		; |
		ADD R1, R1, R2		; /

		ADD R6, R6, -2		; \ R0 = eval(str, len)
		STR R0, R6, 0		; |
		STR R1, R6, 1		; |
		LD R2, EVALPTR		; |
		JSRR R2				; |
		LDR R0, R6, 0		; |
		ADD R6, R6, 3		; /

		ST R0, ANS
		HALT

STACK	.fill xf000
ANS		.fill -1
EVALPTR	.fill EVAL
STRING	.stringz "1+4*7*6+0*9+4*8*2+2+4+2*7+3*1"
		.blkw 200

	 	;Initializing stack operations
EVAL	ADD R6, R6, -6 	;Make space for RV, RA, old FP, and local vars i, left, and right
		STR R7, R6, 4	;Store RA onto stack
		STR R5, R6, 3	;Store old FP onto stack
		ADD R5, R6, 2	;Point FP at first local variable

        ;Begin eval
        AND R0, R0, 0	;R0 = i = 0
		STR R0, R5, 0	;Store i on stack

        ;LOOP1 loop condition
LOOP1	LDR R1, R5, 5	;R1 = len
		NOT R1, R1		;Two's complement
		ADD R1, R1, 1
		ADD R0, R0, R1	;i - len
		BRzp BREAK1		;Done looping when (I - len) >= 0

			;LOOP1 loop body
			LDR R0, R5, 0	;Reload i
			LDR R1, R5, 5 	;Reload len
			LDR R2, R5, 4	;R2 = str

			;LOOP1 if condition
			ADD R2, R2, R0		;R2 = str + i
			LDR R2, R2, 0		;R2 = *(str + i)
			ADD R2, R2, -16		;char - 43 (ASCII value for '+')
			ADD R2, R2, -16
			ADD R2, R2, -11
			BRnp SKIP1			;Skip if when current char =/= '+'

				;LOOP1 if body
				ADD R6, R6, -2 	;Make space for args on stack
				LDR R2, R5, 4	;Reload str
				STR R0, R6, 1	;Store arg len = i on stack
				STR R2, R6, 0	;Store arg *str = str on stack
				JSR EVAL 		;EVAL(str, i)
				LDR R3, R6, 0	;Load RV from SP
				STR R3, R5, -1 	;Store local var left on stack

				ADD R6, R6, 3	;Destroy previous recurse and point SP to top of stack
				LDR R0, R5, 0	;Reload i
				LDR R1, R5, 5 	;Reload len
				LDR R2, R5, 4	;R2 = str

				ADD R6, R6, -2 	;Make space for args on stack
				ADD R2, R2, R0	;str + i
				ADD R2, R2, 1 	;str + i + 1
				STR R2, R6, 0	;Store arg len = len - i - 1 on stack
				NOT R0, R0		;Two's complement
				ADD R0, R0, 1
				ADD R1, R1, R0	;len - i
				ADD R1, R1, -1 	;len - i - 1
				STR R1, R6, 1 	;Store arg *str = str + i + 1
				JSR EVAL 		;EVAL(str + i + 1, len - i - 1)
				LDR R4, R6, 0	;Load RV from SP
				STR R4, R5, -2 	;Store local var right on stack

				LDR R3, R5, -1 	;Reload left
				LDR R4, R5, -2 	;Reload right
				ADD R0, R3, R4	;R0 = left + right
				BR DONE

SKIP1		LDR R0, R5, 0		;Reload i
			ADD R0, R0, 1 		;Increment i
			STR R0, R5, 0 		;Save incremented i
			BR LOOP1			;Loop LOOP1

BREAK1  AND R0, R0, 0	;Reinitialize i
		STR R0, R5, 0	;Store i on stack

		;LOOP2 loop condition
LOOP2	LDR R1, R5, 5	;R1 = len
		NOT R1, R1		;Two's complement
		ADD R1, R1, 1
		ADD R0, R0, R1	;i - len
		BRzp BREAK2		;Done looping when (I - len) >= 0

			;LOOP2 loop body
			LDR R0, R5, 0	;Reload i
			LDR R1, R5, 5 	;Reload len
			LDR R2, R5, 4	;R2 = str

			;LOOP2 if condition
			ADD R2, R2, R0		;R2 = str + i
			LDR R2, R2, 0		;R2 = *(str + i)
			ADD R2, R2, -16		;char - 42 (ASCII value for '*')
			ADD R2, R2, -16
			ADD R2, R2, -10
			BRnp SKIP2			;Skip if when current char =/= '*'

				;LOOP2 if body
				ADD R6, R6, -2 	;Make space for args on stack
				LDR R2, R5, 4	;Reload str
				STR R0, R6, 1	;Store arg len = i on stack
				STR R2, R6, 0	;Store arg *str = str on stack
				JSR EVAL 		;EVAL(str, i)
				LDR R3, R6, 0	;Load RV from SP
				STR R3, R5, -1 	;Store local var left on stack

				ADD R6, R6, 3	;Destroy previous recurse and point SP to top of stack
				LDR R0, R5, 0	;Reload i
				LDR R1, R5, 5 	;Reload len
				LDR R2, R5, 4	;R2 = str

				ADD R6, R6, -2 	;Make space for args on stack
				ADD R2, R2, R0	;str + i
				ADD R2, R2, 1 	;str + i + 1
				STR R2, R6, 0	;Store arg len = len - i - 1 on stack
				NOT R0, R0		;Two's complement
				ADD R0, R0, 1
				ADD R1, R1, R0	;len - i
				ADD R1, R1, -1 	;len - i - 1
				STR R1, R6, 1 	;Store arg *str = str + i + 1
				JSR EVAL 		;EVAL(str + i + 1, len - i - 1)
				LDR R4, R6, 0	;Load RV from SP
				STR R4, R5, -2 	;Store local var right on stack

				LDR R3, R5, -1 	;Reload left
				LDR R4, R5, -2 	;Reload right

				AND R0, R0, 0	;Initialize product
MULT			ADD R0, R0, R3	;R0 += R3
				ADD R4, R4, -1 	;Decrement counter
				BRp MULT		;If (counter > 0) -> LOOP

				BR DONE

SKIP2		LDR R0, R5, 0		;Reload i
			ADD R0, R0, 1 		;Increment i
			STR R0, R5, 0 		;Save incremented i
			BR LOOP2	

BREAK2	LDR R0, R5, 4		;R0 = str
		LDR R0, R0, 0		;R0 = *str
		ADD R0, R0, -16		;*str - '0'
		ADD R0, R0, -16
		ADD R0, R0, -16

		;Finalizing stack operations
DONE    STR R0, R5, 3   ;Store RV onto the stack
	    LDR R7, R5, 2   ;Restore RA into R7
	    ADD R6, R5, 3   ;Point SP at the RV
	    LDR R5, R5, 1   ;Restore old FP
	    RET 			;Return

.end
