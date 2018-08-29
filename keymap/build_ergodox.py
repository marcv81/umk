import keymap

layer1 = """

 Esc    1     2     3     4     5    ---   ---    6     7     8     9     0     =
  '     Q     W     E     R     T     [     \     Y     U     I     O     P     -
LCtrl   A     S     D     F     G    xxx   xxx    H     J     K     L     ;   RCtrl
LShft   Z     X     C     V     B     ]     `     N     M     ,     .     /   RShft
Layer LSup  LAlt   ---   ---   xxx   xxx   xxx   xxx   ---   ---   RAlt  RSup Layer
 xxx   ---   Tab  Space  Mute  Vol-  Vol+  ---   ---   ---  BckSp Enter  ---   xxx

"""

layer2 = """

 ---   F1    F2    F3    F4    F5    ---   ---    F6    F7    F8    F9   F10   F11
 ---   ---   ---  PgUp   ---   ---   ---   ---   ---   ---    Up   ---   ---   F12
LCtrl  ---  Home  PgDn   End   ---   xxx   xxx   ---   Left  Down Right  ---  RCtrl
LShft  ---   ---   ---   ---   ---   ---   ---   ---   ---   ---   ---   ---  RShft
Layer LSup  LAlt   ---   ---   xxx   xxx   xxx   xxx   ---   ---   RAlt  RSup Layer
 xxx   ---   ---   ---   ---   ---   ---   ---   ---   ---   ---   ---   ---   xxx

"""

keymap.Keymap(layer1, layer2, rows=6, columns=14, reverse_rows=True).render()