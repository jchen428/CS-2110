;;==============================================
;; NAME: Jesse Chen
;;==============================================

.orig x3000

		LD R0, A		;Load A in R0
		LD R1, B		;Load B in R1

		TRAP x30 		;Exectue multiplication

		HALT

		A		.fill 24
		B		.fill 37

.end

.orig x30

	.fill x5000

.end

.orig x5000
		
		ST R0, NUM			;Store R0 as number being multiplied
		ST R1, INDEX 		;Store R1 as current index
		AND R0, R0, 0		;R0 = 0

LOOP	LD R1, NUM 			;R1 = A
		ADD R0, R0, R1		;R0 += R1
		LD R1, INDEX 		;R1 = INDEX
		ADD R1, R1, -1		;INDEX--
		ST R1, INDEX 		;Store R1 INDEX
		
		BRp LOOP			;If (INDEX > 0) -> LOOP

		RET

		NUM 	.fill 0
		INDEX 	.fill 0
		
.end