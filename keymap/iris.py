import keymap

layer0 = """

 Esc    1     2     3     4     5     6     7     8     9     0     =
  '     Q     W     E     R     T     Y     U     I     O     P     -
LCtrl   A     S     D     F     G     H     J     K     L     ;   RCtrl
LShft   Z     X     C     V     B     N     M     ,     .     /   RShft
 ---   ---  LSup  Layer Space  Tab  Enter BckSp Layer  RAlt  ---   ---

"""

layer1 = """

 ---   F1    F2    F3    F4    F5     F6    F7    F8    F9   F10   F11
 ---   ---    `   PgUp    \    ---   ---    [     Up    ]    ---   F12
LCtrl  ---  Home  PgDn   End   ---   ---   Left  Down Right  ---  RCtrl
LShft  ---  Mute  Vol-   Vol+  ---   ---   ---   ---   ---   ---  RShft
 ---   ---  LSup  Layer  ---   ---   ---   ---  Layer  RAlt  ---   ---

"""

keymap.Keymap((layer0, layer1), rows=5, columns=12).render()
