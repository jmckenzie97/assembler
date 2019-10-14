
    .text
main:   lw     $t0, 4($s1)
    lw  $t1, 4($s2)
    ori $t2, $0, 3
    add $t2, $t0, $t1
    sw  $t2, 0($s2)
	la	$t1,L1
	ori	$s0,$0,0
	ori	$s1,$s1,0
	la	$s0,A1
	lw	$t6,0($s0)
	addi	$t7,$t6,1
	sw	$t7,0($s0)
	.data
	L1:	.word	-1
	A1:	.space	4
	A2:	.word 	5