
    .text
main:   lw     $t0, 4($s1)
    lw  $t1, 4($s2)
    ori $t2, $0, 3
    add $t2, $t0, $t1
    sw  $t2, 0($s2)
	la	$t1,L1
	ori	$s0,$0,0
	ori	$s1,$s1,0
	.data
	L1:	.word	-1