import keymap

layer1 = """

 Esc    1     2     3     4     5   Mute   Vol-  Vol+   6     7     8     9     0     =
  '     Q     W     E     R     T    ---   ---   ---    Y     U     I     O     P     -
Ctrl    A     S     D     F     G     [    ---    ]     H     J     K     L     ;    Ctrl
Shift   Z     X     C     V     B     `    ---    \     N     M     ,     .     /   Shift
Layer Super  Alt   ---   ---  Space  Tab   ---   Enter BckSp ---   ---   Alt  Super Layer

"""

layer2 = """

 ---   F1    F2    F3    F4    F5    F6    ---    F7    F8    F9   F10   F11   F12   ---
 ---   ---   ---  PgUp   ---   ---   ---   ---   ---   ---   ---    Up   ---   ---   ---
Ctrl   ---  Home  PgDn   End   ---   ---   ---   ---   ---   Left  Down Right  ---   Ctrl
Shift  ---   ---   ---   ---   ---   ---   ---   ---   ---   ---   ---   ---   ---  Shift
Layer Super  Alt   ---   ---   ---   ---   ---   ---   ---   ---   ---   Alt  Super Layer

"""

keymap.Keymap(layer1, layer2, rows=5, columns=15).render()
