import keymap

layer0 = """

 Esc    1     2     3     4     5    F10   F11   F12    6     7     8     9     0     =
  '     Q     W     E     R     T    F7    F8    F9     Y     U     I     O     P     -
LCtrl   A     S     D     F     G    F4    F5    F6     H     J     K     L     ;   RCtrl
LShft   Z     X     C     V     B    F1    F2    F3     N     M     ,     .     /   RShft
Layer LSup  LAlt  Space  ---  Space  Mute  Vol-  Vol+ BckSp  ---  BckSp  RAlt  RSup Layer

"""

layer1 = """

 ---   ---   ---   ---   ---   ---   ---   ---   ---   ---   ---   ---   ---   ---   ---
 ---   ---    `   PgUp    \    ---   ---   ---   ---   ---    [     Up    ]    ---   ---
LCtrl  ---  Home  PgDn   End   ---   ---   ---   ---   ---   Left  Down Right  ---  RCtrl
LShft  ---   ---   ---   ---   ---   ---   ---   ---   ---   ---   ---   ---   ---  RShft
Layer LSup  LAlt   Tab   ---   Tab   ---   ---   ---  Enter  ---  Enter  RAlt  RSup Layer

"""

keymap.Keymap((layer0, layer1), rows=5, columns=15).render()
