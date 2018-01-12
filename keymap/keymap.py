import re

keycodes = {

    # General keys
    "A":     (0x01, 0x04),        # a and A
    "B":     (0x01, 0x05),        # b and B
    "C":     (0x01, 0x06),        # c and C
    "D":     (0x01, 0x07),        # d and D
    "E":     (0x01, 0x08),        # e and E
    "F":     (0x01, 0x09),        # f and F
    "G":     (0x01, 0x0a),        # g and G
    "H":     (0x01, 0x0b),        # h and H
    "I":     (0x01, 0x0c),        # i and I
    "J":     (0x01, 0x0d),        # j and J
    "K":     (0x01, 0x0e),        # k and K
    "L":     (0x01, 0x0f),        # l and L
    "M":     (0x01, 0x10),        # m and M
    "N":     (0x01, 0x11),        # n and N
    "O":     (0x01, 0x12),        # o and O
    "P":     (0x01, 0x13),        # p and P
    "Q":     (0x01, 0x14),        # q and Q
    "R":     (0x01, 0x15),        # r and R
    "S":     (0x01, 0x16),        # s and S
    "T":     (0x01, 0x17),        # t and T
    "U":     (0x01, 0x18),        # u and U
    "V":     (0x01, 0x19),        # v and V
    "W":     (0x01, 0x1a),        # w and W
    "X":     (0x01, 0x1b),        # x and X
    "Y":     (0x01, 0x1c),        # y and Y
    "Z":     (0x01, 0x1d),        # z and Z
    "1":     (0x01, 0x1e),        # 1 and !
    "2":     (0x01, 0x1f),        # 2 and @
    "3":     (0x01, 0x20),        # 3 and #
    "4":     (0x01, 0x21),        # 4 and $
    "5":     (0x01, 0x22),        # 5 and %
    "6":     (0x01, 0x23),        # 6 and ^
    "7":     (0x01, 0x24),        # 7 and &
    "8":     (0x01, 0x25),        # 8 and *
    "9":     (0x01, 0x26),        # 9 and (
    "0":     (0x01, 0x27),        # 0 and )
    "Enter": (0x01, 0x28),        # Enter
    "Esc":   (0x01, 0x29),        # Escape
    "BckSp": (0x01, 0x2a),        # Backspace
    "Tab":   (0x01, 0x2b),        # Tab
    "Space": (0x01, 0x2c),        # Space
    "-":     (0x01, 0x2d),        # - and _
    "=":     (0x01, 0x2e),        # = and +
    "[":     (0x01, 0x2f),        # [ and {
    "]":     (0x01, 0x30),        # ] and }
    "\\":    (0x01, 0x31),        # \ and |
    ";":     (0x01, 0x33),        # ; and :
    "'":     (0x01, 0x34),        # ' and "
    "`":     (0x01, 0x35),        # ` and ~
    ",":     (0x01, 0x36),        # , and <
    ".":     (0x01, 0x37),        # . and >
    "/":     (0x01, 0x38),        # / and ?
    "F1":    (0x01, 0x3a),        # F1
    "F2":    (0x01, 0x3b),        # F2
    "F3":    (0x01, 0x3c),        # F3
    "F4":    (0x01, 0x3d),        # F4
    "F5":    (0x01, 0x3e),        # F5
    "F6":    (0x01, 0x3f),        # F6
    "F7":    (0x01, 0x40),        # F7
    "F8":    (0x01, 0x41),        # F8
    "F9":    (0x01, 0x42),        # F9
    "F10":   (0x01, 0x43),        # F10
    "F11":   (0x01, 0x44),        # F11
    "F12":   (0x01, 0x45),        # F12
    "Ins":   (0x01, 0x49),        # Insert
    "Home":  (0x01, 0x4a),        # Home
    "PgUp":  (0x01, 0x4b),        # Page Up
    "Del":   (0x01, 0x4c),        # Delete
    "End":   (0x01, 0x4d),        # End
    "PgDn":  (0x01, 0x4e),        # Page Down
    "Right": (0x01, 0x4f),        # Right Arrow
    "Left":  (0x01, 0x50),        # Left Arrow
    "Down":  (0x01, 0x51),        # Down Arrow
    "Up":    (0x01, 0x52),        # Up Arrow

    # Modifer keys
    "Ctrl":  (0x02, 0b00000001),  # Control
    "Shift": (0x02, 0b00000010),  # Shift
    "Alt":   (0x02, 0b00000100),  # Alt
    "Super": (0x02, 0b00001000),  # Super

    # Layer keys
    "Layer": (0x03, 2),           # Select layer 2

}


class Layer:

    def __init__(self, legends):
        self.legends = legends

    def create(string, rows, columns, reverse_rows=False):
        # Read from string
        legends_array = []
        regex = re.compile('\S+')
        for line in string.split("\n"):
            legends_row = regex.findall(line)
            if len(legends_row) > 0:
                legends_array.append(legends_row)
        # Verify dimensions
        assert len(legends_array) == rows
        for legends_row in legends_array:
            assert len(legends_row) == columns
        # Reverse rows order
        if reverse_rows:
            legends_array = list(reversed(legends_array))
        # Create and return legends
        legends = [l for legends_row in legends_array for l in legends_row]
        return Layer(legends)


class Keymap:

    def __init__(self, layer1, layer2, rows, columns, reverse_rows):
        self.layer1 = Layer.create(
            layer1, rows, columns, reverse_rows)
        self.layer2 = Layer.create(
            layer2, rows, columns, reverse_rows)

    @staticmethod
    def format_legend(l):
        if l in keycodes:
            return "0x%02x, 0x%02x," % keycodes[l]
        else:
            return "0x00, 0x00,"

    def render(self):
        print("static const PROGMEM uint8_t keymap[4 * MATRIX_KEYS] =")
        print("{")
        print("\t// Layer 1")
        for l in self.layer1.legends:
            print("\t" + Keymap.format_legend(l))
        print()
        print("\t// Layer 2")
        for l in self.layer2.legends:
            print("\t" + Keymap.format_legend(l))
        print("};")
