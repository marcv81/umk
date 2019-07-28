import keymap

mapping = """

 0,0   0,1   0,2   0,3   0,4   0,5                     0,6   0,7   0,8   0,9   0,10  0,11
 1,0   1,1   1,2   1,3   1,4   1,5                     1,6   1,7   1,8   1,9   1,10  1,11
 2,0   2,1   2,2   2,3   2,4   2,5                     2,6   2,7   2,8   2,9   2,10  2,11
 3,0   3,1   3,2   3,3   3,4   3,5   4,2         4,9   3,6   3,7   3,8   3,9   3,10  3,11
                         4,3   4,4   4,5         4,6   4,7   4,8

"""

layer0 = """

 Esc    1     2     3     4     5                       6     7     8     9     0     =
  '     Q     W     E     R     T                       Y     U     I     O     P     -
LCtrl   A     S     D     F     G                       H     J     K     L     ;   RCtrl
LShft   Z     X     C     V     B   LSup         RAlt   N     M     ,     .     /   RShft
                        Layer Space  Tab        Enter BckSp Layer

"""

layer1 = """

 ---   F1    F2    F3    F4    F5                       F6    F7    F8    F9   F10   F11
 ---   ---    `   PgUp    \    ---                     ---    [     Up    ]    ---   F12
LCtrl  ---  Home  PgDn   End   ---                     ---   Left  Down Right  ---  RCtrl
LShft  ---  Mute  Vol-   Vol+  ---  LSup         RAlt  ---   ---   ---   ---   ---  RShft
                        Layer  ---   ---         ---   ---  Layer

"""

print(keymap.keymap_str(mapping, (layer0, layer1), rows=5, columns=12))
