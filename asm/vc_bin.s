.include "macros.inc"
.section .text

.balign 16
glabel osEepromReadVC
.incbin "bin/vc/osEepromReadVC.bin"

.balign 16
glabel osEepromWriteVC
.incbin "bin/vc/osEepromWriteVC.bin"

.balign 16
glabel __osEepStatusVC
.incbin "bin/vc/osEepStatusVC.bin"
