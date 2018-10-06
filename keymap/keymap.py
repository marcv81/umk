import re

legends = {

    "---":   ("Normal", 0),

    # Normal keys
    "A":     ("Normal", 0x04),          # a and A
    "B":     ("Normal", 0x05),          # b and B
    "C":     ("Normal", 0x06),          # c and C
    "D":     ("Normal", 0x07),          # d and D
    "E":     ("Normal", 0x08),          # e and E
    "F":     ("Normal", 0x09),          # f and F
    "G":     ("Normal", 0x0a),          # g and G
    "H":     ("Normal", 0x0b),          # h and H
    "I":     ("Normal", 0x0c),          # i and I
    "J":     ("Normal", 0x0d),          # j and J
    "K":     ("Normal", 0x0e),          # k and K
    "L":     ("Normal", 0x0f),          # l and L
    "M":     ("Normal", 0x10),          # m and M
    "N":     ("Normal", 0x11),          # n and N
    "O":     ("Normal", 0x12),          # o and O
    "P":     ("Normal", 0x13),          # p and P
    "Q":     ("Normal", 0x14),          # q and Q
    "R":     ("Normal", 0x15),          # r and R
    "S":     ("Normal", 0x16),          # s and S
    "T":     ("Normal", 0x17),          # t and T
    "U":     ("Normal", 0x18),          # u and U
    "V":     ("Normal", 0x19),          # v and V
    "W":     ("Normal", 0x1a),          # w and W
    "X":     ("Normal", 0x1b),          # x and X
    "Y":     ("Normal", 0x1c),          # y and Y
    "Z":     ("Normal", 0x1d),          # z and Z
    "1":     ("Normal", 0x1e),          # 1 and !
    "2":     ("Normal", 0x1f),          # 2 and @
    "3":     ("Normal", 0x20),          # 3 and #
    "4":     ("Normal", 0x21),          # 4 and $
    "5":     ("Normal", 0x22),          # 5 and %
    "6":     ("Normal", 0x23),          # 6 and ^
    "7":     ("Normal", 0x24),          # 7 and &
    "8":     ("Normal", 0x25),          # 8 and *
    "9":     ("Normal", 0x26),          # 9 and (
    "0":     ("Normal", 0x27),          # 0 and )
    "Enter": ("Normal", 0x28),          # Enter
    "Esc":   ("Normal", 0x29),          # Escape
    "BckSp": ("Normal", 0x2a),          # Backspace
    "Tab":   ("Normal", 0x2b),          # Tab
    "Space": ("Normal", 0x2c),          # Space
    "-":     ("Normal", 0x2d),          # - and _
    "=":     ("Normal", 0x2e),          # = and +
    "[":     ("Normal", 0x2f),          # [ and {
    "]":     ("Normal", 0x30),          # ] and }
    "\\":    ("Normal", 0x31),          # \ and |
    ";":     ("Normal", 0x33),          # ; and :
    "'":     ("Normal", 0x34),          # ' and "
    "`":     ("Normal", 0x35),          # ` and ~
    ",":     ("Normal", 0x36),          # , and <
    ".":     ("Normal", 0x37),          # . and >
    "/":     ("Normal", 0x38),          # / and ?
    "F1":    ("Normal", 0x3a),          # F1
    "F2":    ("Normal", 0x3b),          # F2
    "F3":    ("Normal", 0x3c),          # F3
    "F4":    ("Normal", 0x3d),          # F4
    "F5":    ("Normal", 0x3e),          # F5
    "F6":    ("Normal", 0x3f),          # F6
    "F7":    ("Normal", 0x40),          # F7
    "F8":    ("Normal", 0x41),          # F8
    "F9":    ("Normal", 0x42),          # F9
    "F10":   ("Normal", 0x43),          # F10
    "F11":   ("Normal", 0x44),          # F11
    "F12":   ("Normal", 0x45),          # F12
    "Ins":   ("Normal", 0x49),          # Insert
    "Home":  ("Normal", 0x4a),          # Home
    "PgUp":  ("Normal", 0x4b),          # Page Up
    "Del":   ("Normal", 0x4c),          # Delete
    "End":   ("Normal", 0x4d),          # End
    "PgDn":  ("Normal", 0x4e),          # Page Down
    "Right": ("Normal", 0x4f),          # Right Arrow
    "Left":  ("Normal", 0x50),          # Left Arrow
    "Down":  ("Normal", 0x51),          # Down Arrow
    "Up":    ("Normal", 0x52),          # Up Arrow
    "Mute":  ("Normal", 0x7f),          # Mute
    "Vol+":  ("Normal", 0x80),          # Volume Up
    "Vol-":  ("Normal", 0x81),          # Volume Down

    # Modifers
    "LCtrl": ("Modifier", 0b00000001),  # Left Control
    "LShft": ("Modifier", 0b00000010),  # Left Shift
    "LAlt":  ("Modifier", 0b00000100),  # Left Alt
    "LSup":  ("Modifier", 0b00001000),  # Left Super
    "RCtrl": ("Modifier", 0b00010000),  # Right Control
    "RShft": ("Modifier", 0b00100000),  # Right Shift
    "RAlt":  ("Modifier", 0b01000000),  # Right Alt
    "RSup":  ("Modifier", 0b10000000),  # Right Super

    # Layer keys
    "Layer": ("Layer", 1),              # Select layer 1

}

# Read line/space separated tokens into an array of arrays
# Apply a conversion function to each token if supplied
def read_tokens(string, convert=None):
    regex = re.compile('\S+')
    tokens = []
    for line in string.split("\n"):
        row = tuple(regex.findall(line))
        if convert is not None:
            row = tuple(convert(token) for token in row)
        if len(row) > 0:
            tokens.append(row)
    return tuple(tokens)

# Create mapping from string
def create_mapping(string, rows, columns):
    matrix = set()
    def convert(token):
        # Validate row and column
        row, column = tuple(int(i) for i in token.split(','))
        assert (row < rows) and (column < columns)
        assert (row, column) not in matrix
        matrix.add((row, column))
        # Return matrix index
        return row * columns + column
    return read_tokens(string, convert)

# Create layer from string
def create_layer(string, mapping, rows, columns):
    # Create keycodes from string
    def convert(token):
        assert token in legends
        return legends[token]
    keycodes = read_tokens(string, convert)
    # Iterate over pairs from two arrays
    # of arrays with the same shape
    def iterate_pairs(a, b):
        def iterate(a, b):
            assert len(a) == len(b)
            for i in range(len(a)):
                yield (a[i], b[i])
        for ai, bi in iterate(a, b):
            for aij, bij in iterate(ai, bi):
                yield aij, bij
    # Apply mapping to keycodes
    layer = [legends['---'] for _ in range(rows * columns)]
    for index, keycode in iterate_pairs(mapping, keycodes):
        layer[index] = keycode
    # Return layer
    return layer

class Keymap:

    def __init__(self, mapping, layers, rows, columns):
        mapping = create_mapping(mapping, rows, columns)
        self.layers = [
            create_layer(layer, mapping, rows, columns)
            for layer in layers]

    def render_layer(self, i):
        print("\t// Layer %d" % i)
        print("\t{")
        for keycode in self.layers[i]:
            print("\t\t{ .type = %s, .value = 0x%02x }," % keycode)
        print("\t},")

    def render(self):
        print("static const PROGMEM keycode_t keymap[2][MATRIX_KEYS] =")
        print("{")
        for i in range(len(self.layers)):
            self.render_layer(i)
            if i != len(self.layers) - 1:
                print()
        print("};")
