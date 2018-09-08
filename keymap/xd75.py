import keymap

layer1 = """

 Esc    1     2     3     4     5   Mute   Vol-  Vol+   6     7     8     9     0     =
  '     Q     W     E     R     T    ---   ---   ---    Y     U     I     O     P     -
LCtrl   A     S     D     F     G     [    ---    \     H     J     K     L     ;   RCtrl
LShft   Z     X     C     V     B     ]    ---    `     N     M     ,     .     /   RShft
Layer LSup  LAlt  Space  xxx  Space  Tab   ---   Enter BckSp xxx  BckSp  RAlt  RSup Layer

"""

layer2 = """

 ---   F1    F2    F3    F4    F5    ---   ---   ---    F6    F7    F8    F9   F10   F11
 ---   ---   ---  PgUp   ---   ---   ---   ---   ---   ---   ---    Up   ---   ---   F12
LCtrl  ---  Home  PgDn   End   ---   ---   ---   ---   ---   Left  Down Right  ---  RCtrl
LShft  ---   ---   ---   ---   ---   ---   ---   ---   ---   ---   ---   ---   ---  RShft
Layer LSup  LAlt   ---   ---   ---   ---   ---   ---   ---   ---   ---   RAlt  RSup Layer

"""

keymap.Keymap(layer1, layer2, rows=5, columns=15).render()
