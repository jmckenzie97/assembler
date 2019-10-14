
    .text
main:   lw  $t0, 4($s1)
    la	$t1, 4($s2)
    ori $t2, $0, 3
    add $t2, $t0, $t1
    sw  $t2, 0($s2)