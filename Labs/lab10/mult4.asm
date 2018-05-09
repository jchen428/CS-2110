.orig x3000

  LD R0, N
  LD R1, ANS
  ADD R0, R0, -4
  BRp -2
  BRz 4
  BRn 5

  HALT

  N .fill 20
  ANS .fill -1

  ADD R1, R1, 2
  HALT

  ADD R1, R1, 1
  HALT
.end
