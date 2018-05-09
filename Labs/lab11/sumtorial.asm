;;===============================
;;Name: Jesse Chen
;;===============================

.orig x3000

		LD R0, N        ;Load N into R0
		LD R1, ANS		;Load ANS into R1
		AND R2, R2, 0	;Initialize counter into R2
		;ADD R2, R2, 1	;counter = 1

LOOP	ADD R1, R1, R2	;ANS += counter
		ADD R2, R2, 1	;counter++

		ADD R3, R2, 0	;Copy counter into R3
		NOT R3, R3		;Begin two's complement
		ADD R3, R3, 1	;Finish two's complement for -counter
		ADD R3, R0, R3	;N - counter
		BRn END			;if (N - counter >= 0) END
		BR LOOP			;else LOOP

END		ST R1, ANS		;Store answer in ANS

		HALT

N       .fill 4
ANS     .fill 0

.end