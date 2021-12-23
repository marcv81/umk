import keymap

mapping = """

 0,0   0,1   0,2   0,3   0,4   0,5   0,6   0,7   0,8   0,9   0,10  0,11  0,12  0,13  0,14
 1,0   1,1   1,2   1,3   1,4   1,5   1,6   1,7   1,8   1,9   1,10  1,11  1,12  1,13  1,14
 2,0   2,1   2,2   2,3   2,4   2,5   2,6   2,7   2,8   2,9   2,10  2,11  2,12  2,13  2,14
 3,0   3,1   3,2   3,3   3,4   3,5   3,6   3,7   3,8   3,9   3,10  3,11  3,12  3,13  3,14
 4,0   4,1   4,2   4,3         4,5   4,6   4,7   4,8   4,9         4,11  4,12  4,13  4,14

"""

layer0 = """

 Esc    1     2     3     4     5   PrtSc ScrLk Pause   6     7     8     9     0     =
  '     Q     W     E     R     T    ---  Timer  ---    Y     U     I     O     P     -
CtrlL   A     S     D     F     G   SupL  CpsLk SupR    H     J     K     L     ;   CtrlR
ShftL   Z     X     C     V     B   AltL   Up   AltR    N     M     ,     .     /   ShftR
Layer   `     \   Space       Space Left  Down  Right BckSp       BckSp   [     ]   Layer

"""

layer1 = """

Mute   ---   F10   F11   F12   ---   ---   ---   ---   ---   ---   ---   ---   ---  Vol+
 ---   ---   F7    F8    F9    ---   ---   ---   ---  PgUp  Home   Up    End   Ins  Vol-
CtrlL  ---   F4    F5    F6    ---  SupL   ---  SupR  PgDn  Left  Down  Right  Del  CtrlR
ShftL  ---   F1    F2    F3    ---  AltL   ---  AltR   ---   ---   ---   ---   ---  ShftR
Layer  ---   ---   Tab         Tab   ---   ---   ---  Enter       Enter  ---   ---  Layer

"""

print(keymap.keymap_str(mapping, (layer0, layer1), rows=5, columns=15))
