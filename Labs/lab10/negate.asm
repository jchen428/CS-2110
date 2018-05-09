.orig x3000

  LD R0, N
  NOT R0, R0
  ADD R0, R0, 1
  ST R0, ANS

  HALT

  N .fill 50
  ANS .fill 0

.end
