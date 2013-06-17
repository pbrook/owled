PCBNEW-LibModule-V1  Sat 11 May 2013 12:55:11 BST
# encoding utf-8
$INDEX
LEDEND
PAD2010
PAD2510
SOT23-13
$EndINDEX
$MODULE SOT23-13
Po 0 0 0 15 518BD89B 00000000 ~~
Li SOT23-13
Cd SOT32 pins 1 and 3
Kw SOT23-13
Sc 00000000
AR /518BD565
Op 0 0 0
T0 787 -39 300 300 900 30 N V 21 N "D2"
T1 25 0 197 197 0 30 N V 21 N "SCHOTTY"
DS -200 300 -500 100 50 21
DS 500 300 -525 300 50 21
DS -525 300 -525 -300 50 21
DS -525 -300 500 -300 50 21
DS 500 -300 500 300 50 21
$PAD
Sh "2" R 276 394 0 0 0
Dr 0 0 0
At SMD N 00888000
Ne 2 "N-000002"
Po 0 -500
$EndPAD
$PAD
Sh "3" R 276 394 0 0 0
Dr 0 0 0
At SMD N 00888000
Ne 0 ""
Po 375 500
$EndPAD
$PAD
Sh "1" R 276 394 0 0 0
Dr 0 0 0
At SMD N 00888000
Ne 1 "N-000001"
Po -375 500
$EndPAD
$SHAPE3D
Na "smd/SOT23_6.wrl"
Sc 0.110000 0.110000 0.110000
Of 0.000000 0.000000 0.000000
Ro 0.000000 0.000000 -180.000000
$EndSHAPE3D
$EndMODULE  SOT23-13
$MODULE LEDEND
Po 0 0 0 15 518BF326 00000000 ~~
Li LEDEND
Cd double sided pads for LED
Sc 00000000
AR LEDEND
Op 0 0 0
T0 0 0 600 600 0 120 N I 21 N "LEDEND"
T1 0 0 600 600 0 120 N V 21 N "VAL**"
$PAD
Sh "1" R 2000 1000 0 0 0
Dr 0 0 0
At SMD N 00888000
Ne 0 ""
Po 0 1000
$EndPAD
$PAD
Sh "2" R 2000 1000 0 0 0
Dr 0 0 0
At SMD N 00440001
Ne 0 ""
Po 0 1000
$EndPAD
$PAD
Sh "3" R 2000 1000 0 0 0
Dr 0 0 0
At SMD N 00440001
Ne 0 ""
Po 0 -1000
$EndPAD
$PAD
Sh "4" R 2000 1000 0 0 0
Dr 0 0 0
At SMD N 00888000
Ne 0 ""
Po 0 -1000
$EndPAD
$EndMODULE  LEDEND
$MODULE PAD2010
Po 0 0 0 15 518E2FDA 00000000 ~~
Li PAD2010
Cd .1x.2 pad
Kw DEV
Sc 00000000
AR 1pin
Op 0 0 0
T0 0 0 400 400 0 100 N I 21 N "PAD2010"
T1 -1750 0 400 400 0 100 N V 21 N "P***"
$PAD
Sh "1" R 2000 1000 0 0 0
Dr 0 0 0
At SMD N 00CC8000
Ne 0 ""
Po 0 0
$EndPAD
$EndMODULE  PAD2010
$MODULE PAD2510
Po 0 0 0 15 518E3168 00000000 ~~
Li PAD2510
Cd .1x.25 pad
Kw DEV
Sc 00000000
AR 1pin
Op 0 0 0
T0 0 0 400 400 0 100 N I 21 N "PAD2510"
T1 -1750 0 400 400 0 100 N V 21 N "P*"
$PAD
Sh "1" R 2500 1000 0 0 0
Dr 0 0 0
At SMD N 00CC8000
Ne 0 ""
Po 0 0
$EndPAD
$EndMODULE  PAD2510
$EndLIBRARY