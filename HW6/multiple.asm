;;===============================
;;Name: Jesse Chen
;;===============================

.orig x3000

        LD R0, A        ;Load A into R0
        LD R1, B        ;Load B into R1
        LD R2, A        ;Load copy of A into R2
        LD R3, ANSWER   ;Load ANSWER into R3
        NOT R1, R1      ;B = !B begin 2's complement
        ADD R1, R1, 1   ;B++ finish 2's complement

        ;Test loop conditions with copy of A -> A'
TEST    ADD R2, R2, R1  ;Use 2's complement of B for A' = A' - B
        BRnz END        ;If A <= 0, end loop

        ;While loop body
        ADD R0, R0, R1  ;Use 2's complement of B for A = A - B
        BR TEST         ;End of while loop body, loop back to beginning

        ;Check if a == b
END     ADD R0, R0, R1  ;Use 2's complement of B for A = A - B
        BRnp RETURN     ;If a =/= b, return 0 since ANSWER is already 0

        ADD R3, R3, 1   ;Return 1

RETURN  ST R3, ANSWER   ;Store answer in ANSWER
        HALT

A       .fill 25
B       .fill 5
ANSWER  .fill 0
.end