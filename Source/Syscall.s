.global SC_SetIABR
SC_SetIABR:
    mtspr 0x3F2, r3
    blr

.global SetIABR
SetIABR:
    lis r0, 0x0000
    ori r0, r0, 0x6D00
    sc
    blr

.global SC_SetDABR
SC_SetDABR:
    mtspr 0x3F5, r3
    blr

.global SetDABR
SetDABR:
    lis r0, 0x0000
    ori r0, r0, 0x6D00
    sc
    blr