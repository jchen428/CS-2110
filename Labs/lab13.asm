.orig x3000

	LD R6, STACK
	LD R0, VAL

	ADD R6, R6, -1 	;Make space for arg VAL
	STR R0, R6, 0	;Store value parameter
	JSR FIB			;Call FIB here!

	ST R0, ANS
	HALT

	STACK	.fill xF000
	VAL	.fill 13
	ANS	.blkw 1

		;Initializing stack operations
FIB		ADD R6, R6, -5 	;Make space for RV, RA, OldFP, and local vars recurse1 and recurse2
		STR R7, R6, 3	;Store RA onto stack
		STR R5, R6, 2	;Store OLDFP onto stack
		ADD R5, R6, 1	;Point FP at first local variable

		;Begin actual subroutine
	    ADD R0, R0, -2	;if (VAL == 1 || VAL == 2)
	    BRp	ELSE

	    AND R0, R0, 0	;VAL = 1
	    ADD R0, R0, 1	;Set RV equal to 1
	    BR DONE

ELSE	ADD R6, R6, -1	;Make space for arg
		LDR R0, R5, 4 	;Reload VAL
		ADD R0, R0, -1	;VAL--
		STR R0, R6, 0	;Store arg on stack
		JSR FIB			;fib(n - 1)
		LDR R0, R6, 0	;Load RV from SP
		STR R0, R5, 0	;Store recurse1 = fib(n - 1) on stack
		ADD R6, R5, -1	;Restore SP

		ADD R6, R6, -1	;Make space arg
		LDR R0, R5, 4 	;Reload VAL
		ADD R0, R0, -2	;VAL -= 2
		STR R0, R6, 0	;Store arg on stack
		JSR FIB 		;fib(n - 2)
		LDR R0, R6, 0	;Load RV from SP
		STR R0, R5, -1	;Store recurse1 = fib(n - 2) on stack
		ADD R6, R5, -1	;Restore SP

		LDR R0, R5, 0	;R0 = recurse1
		LDR R1, R5, -1	;R1 = recurse2
		ADD R0, R0, R1 	;R0 = recurse1 + recurse2

		;Finalizing stack operations
DONE    STR R0, R5, 3   ;Store RV onto the stack
	    LDR R7, R5, 2   ;Restore RA into R7
	    ADD R6, R5, 3   ;Point SP at the RV
	    LDR R5, R5, 1   ;Restore old FP
	    RET 			;Return

.end