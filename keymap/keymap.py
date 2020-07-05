import re
from legends import legends


def read_tokens(string):
    regex = re.compile("\S+")
    for line in string.split("\n"):
        yield from regex.findall(line)


def read_mapping(string, rows, columns):
    mapping = []
    for token in read_tokens(string):
        row, column = tuple(int(i) for i in token.split(","))
        assert row < rows and column < columns
        index = row * columns + column
        mapping.append(index)
    assert len(mapping) == len(set(mapping))
    return mapping


def read_layer(mapping, string, rows, columns):
    tokens = tuple(legends[token] for token in read_tokens(string))
    assert len(mapping) == len(tokens)
    layer = [("None", 0) for _ in range(rows * columns)]
    for i in range(len(mapping)):
        layer[mapping[i]] = tokens[i]
    return layer


def layer_str(layer):
    lines = []
    lines.append("    {")
    for values in layer:
        lines.append("        {.type = %s, .value = 0x%02x}," % values)
    lines.append("    },")
    return "\n".join(lines)


def layers_str(layers):
    lines = []
    lines.append(
        "static const PROGMEM keycode_t keymap[%d][MATRIX_KEYS] = {" % len(layers)
    )
    for i, layer in enumerate(layers):
        lines.append("    // Layer %d" % i)
        lines.append(layer_str(layer))
    lines.append("};")
    return "\n".join(lines)


def keymap_str(mapping_string, layers_strings, rows, columns):
    mapping = read_mapping(mapping_string, rows, columns)
    layers = tuple(
        read_layer(mapping, layer_string, rows, columns)
        for layer_string in layers_strings
    )
    lines = []
    lines.append(layers_str(layers))
    return "\n".join(lines)
