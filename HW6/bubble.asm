;;===============================
;;Name: Jesse Chen
;;===============================

.orig x3000

		;Initializations
		LD R0, K			;Initialize LOOP1 index K = 0 in R0
		LD R1, LENGTH		;Load LENGTH (for-loop condition) into R1
		NOT R1, R1			;Two's complement
		ADD R1, R1, 1 		;-LENGTH
		
		;Outer loop conditions
LOOP1	ST R0, K			;Store R0 (new K) in K
		ADD R0, R0, R1		;K - LENGTH
		BRzp BREAK1			;If (K < LENGTH) -> BREAK1s

		;Outer loop body
		LD R6, ARRAY		;Load ARRAY (x6000) starting index into R6
		ADD R6, R6,1 		;Start at x6001
		AND R2, R2, 0		;Initialize/reset isSorted
		ADD R2, R2, 1 		;isSorted = 1

		LD R0, K			;Reset K
		LD R3, I 			;Initialize LOOP2 index I = 1 in R3

		;Inner loop conditions
LOOP2	ST R3, I 			;Store R3 (new I) in I
		ADD R3, R3, R1		;I - LENGTH
		ADD R3, R3, R0		;I - LENGTH + K
		BRzp BREAK2			;If (I < LENGTH - K) -> BREAK2
		
		;Inner loop body
		;Inner if conditions
		LDR R4, R6, 0		;Load data from ARRAY[I] into R4
		LDR R5, R6, -1 		;Load data from ARRAY[I - 1] into R5
		NOT R5, R5			;Two's complement
		ADD R5, R5, 1 		;-ARRAY[I - 1]
		ADD R4, R4, R5		;ARRAY[I] - ARRAY[I - 1]
		BRzp ELSE			;If (ARRAY[i] < ARRAY[I - 1]) -> ELSE

		;Inner if body
		LDR R4, R6, 0		;Reset ARRAY[I]
		LDR R5, R6, -1 		;Reset ARRAY[I - 1]
		STR R5, R6, 0		;ARRAY[i] = ARRAY[i - 1]
		STR R4, R6, -1 		;ARRAY[i - 1] = R4
		AND R2, R2, 0		;isSorted = 0

		;Increment inner loop
ELSE	LD R3, I
		ADD R3, R3, 1 		;Increment I
		ADD R6, R6, 1 		;Increment array index
		BR LOOP2			;Repeat inner loop

		;Outer if conditions
BREAK2	AND R3, R3, 0 		;Reinitialize I
		ADD R3, R3, 1 		;Reset I = 1
		ADD R2, R2, -1 		;isSorted--
		BRnp INCR 			;If (!isSorted) -> INCR

		;Outer if body
		BR BREAK1 			;If (isSorted) -> BREAK1

		;Increment outer loop
INCR	ADD R0, R0, 1 		;Increment K
		ST R3, I 			;Store R3 (new I) in I
		BR LOOP1			;Repeat outer loop

BREAK1 	HALT

K		.fill 0
I 		.fill 1

ARRAY   .fill x6000
LENGTH  .fill 12
.end

.orig x6000
.fill 28
.fill -50
.fill 7
.fill 0
.fill 216
.fill 4
.fill 15
.fill -82
.fill 34
.fill 101
.fill -5
.fill 61
.end

