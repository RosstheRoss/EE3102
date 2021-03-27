.include "xc.inc"

.text                       ;BP (put the following data in ROM(program memory))
.global _mbs_wait_100us, _mbs_wait_1ms


_mbs_wait_100us:    ;100us delay
    
    clr LATA;
    repeat #1593
    nop
    return
    
_mbs_wait_1ms:      ;1ms delay
    
    repeat #15993
    nop
    return
    




