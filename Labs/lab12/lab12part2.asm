;;==============================================
;; NAME: Jesse Chen
;;==============================================

.orig x3000

		TRAP x40

.end

.orig x40

	.fill x5000

.end

.orig x5000

LOOP	GETC 			;Get character input
		ST R0, CHAR 	;Store current character in CHAR

		ADD R0, R0, -16	;Check if current character is > 64
		ADD R0, R0, -16
		ADD R0, R0, -16
		ADD R0, R0, -16
		BRnz SKIP		;If not -> SKIP

		LD R0, CHAR 	;Reset R0 to current character
		ADD R0, R0, -16	;Check if current character is < 91
		ADD R0, R0, -16
		ADD R0, R0, -16
		ADD R0, R0, -16
		ADD R0, R0, -16
		ADD R0, R0, -11
		BRzp SKIP		;If (CHAR is uppercase) -> CHANGE

CHANGE	LD R0, CHAR 	;Reset R0 to current character
		ADD R0, R0, 15	;Change current uppercase to lowercase
		ADD R0, R0, 15
		ADD R0, R0, 2
		OUT				;Print character
		BR LOOP			;Unconditionally loop forever

SKIP	LD R0, CHAR 	;Reset R0 to current character
		OUT				;Print character
		BR LOOP			;Unconditionally loop forever

		CHAR 	.fill 0
		
.end