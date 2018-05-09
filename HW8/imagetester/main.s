	.cpu arm7tdmi
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 18, 4
	.code	16
	.file	"main.c"
	.global	videoBuffer
	.data
	.align	2
	.type	videoBuffer, %object
	.size	videoBuffer, 4
videoBuffer:
	.word	100663296
	.text
	.align	2
	.global	drawImage3
	.code	16
	.thumb_func
	.type	drawImage3, %function
drawImage3:
	push	{r7, lr}
	sub	sp, sp, #24
	add	r7, sp, #0
	str	r0, [r7, #12]
	str	r1, [r7, #8]
	str	r2, [r7, #4]
	str	r3, [r7]
	mov	r3, #0
	str	r3, [r7, #20]
	b	.L2
.L3:
	ldr	r2, .L5
	ldr	r3, .L5+4
	ldr	r0, [r3]
	ldr	r1, [r7, #8]
	ldr	r3, [r7, #20]
	add	r1, r1, r3
	mov	r3, r1
	lsl	r3, r3, #4
	sub	r3, r3, r1
	lsl	r3, r3, #4
	mov	r1, r3
	ldr	r3, [r7, #12]
	add	r3, r1, r3
	lsl	r3, r3, #1
	add	r3, r0, r3
	str	r3, [r2]
	ldr	r3, .L5+8
	ldr	r2, [r7, #20]
	ldr	r1, [r7, #4]
	mul	r2, r1
	lsl	r2, r2, #1
	ldr	r1, [r7, #32]
	add	r2, r1, r2
	str	r2, [r3]
	ldr	r3, .L5+12
	ldr	r1, [r7, #4]
	ldr	r2, .L5+16
	orr	r2, r2, r1
	str	r2, [r3]
	ldr	r3, [r7, #20]
	add	r3, r3, #1
	str	r3, [r7, #20]
.L2:
	ldr	r2, [r7, #20]
	ldr	r3, [r7]
	cmp	r2, r3
	blt	.L3
	mov	sp, r7
	add	sp, sp, #24
	@ sp needed for prologue
	pop	{r7}
	pop	{r0}
	bx	r0
.L6:
	.align	2
.L5:
	.word	67109080
	.word	videoBuffer
	.word	67109076
	.word	67109084
	.word	-2147483648
	.size	drawImage3, .-drawImage3
	.align	2
	.global	drawImage
	.code	16
	.thumb_func
	.type	drawImage, %function
drawImage:
	push	{r7, lr}
	sub	sp, sp, #24
	add	r7, sp, #8
	str	r0, [r7, #12]
	str	r1, [r7, #8]
	str	r2, [r7, #4]
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #8]
	ldr	r1, [r7, #4]
	str	r1, [sp]
	mov	r0, #0
	mov	r1, #0
	bl	drawImage3
	mov	sp, r7
	add	sp, sp, #16
	@ sp needed for prologue
	pop	{r7}
	pop	{r0}
	bx	r0
	.size	drawImage, .-drawImage
	.align	2
	.global	main
	.code	16
	.thumb_func
	.type	main, %function
main:
	push	{r7, lr}
	add	r7, sp, #0
	mov	r3, #128
	lsl	r3, r3, #19
	ldr	r2, .L12
	strh	r2, [r3]
	ldr	r3, .L12+4
	bl	.L14
.L10:
	b	.L10
.L13:
	.align	2
.L12:
	.word	1027
	.word	draw_an_image
	.size	main, .-main
	.ident	"GCC: (GNU) 4.4.1"
	.code 16
	.align	1
.L14:
	bx	r3
