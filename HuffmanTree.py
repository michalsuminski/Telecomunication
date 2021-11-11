# Algorytm Huffmana https://www.geeksforgeeks.org/huffman-coding-greedy-algo-3/


class node:
    def __init__(self, freq, symbol, left=None, right=None):
        # czestotliwosc symbolu
        self.freq = freq

        # znak
        self.symbol = symbol

        # węzeł po lewej stronie bieżącego węzła
        self.left = left

        # węzeł po prawej stronie bieżącego węzła
        self.right = right

        # kierunek drzewa (0/1)
        self.huff = ''


# Słownik, przechowujący informacje do kodowania znaków
encoded_dict = {}


# funkcja do drukowania kodów Huffmana dla
# wszystkich symboli w nowo utworzonym drzewie Huffmana
def printNodes(node, val=''):
    global encoded_dict
    # kod Huffmana dla bieżącego węzła
    newVal = val + str(node.huff)

    # jeśli węzeł nie jest węzłem krawędziowym
    # przejdź do środka
    if node.left:
        printNodes(node.left, newVal)
    if node.right:
        printNodes(node.right, newVal)

    # jesli wezel jest wezlem krawedziowym,
    # wyswietl kod hufmanna dla znaku
    if (not node.left and not node.right):
        print(f"{node.symbol} -> {newVal}")
        encoded_dict[node.symbol] = newVal


def decodeHuff(root, s):
    cur = root
    chararray = []

    #  Dla każdego znaku, jeśli w węźle wewnętrznym, przesuń się w lewo, jeśli 0, w prawo,
    #  jeśli 1 w lewo. Jeśli w liście (bez dzieci), zapisz dane i wróć do korzenia po przetworzeniu znaku
    for c in s:
        if c == '0' and cur.left:
            cur = cur.left
        elif cur.right:
            cur = cur.right

        if cur.left is None and cur.right is None:
            chararray.append(cur.symbol)
            cur = root

    # Wyswietl caly tekst po odkodowaniu
    str = "".join(chararray)
    return str


# Konwersja znaków i częstotliwości do węzłów drzewa Huffman
def calculate_huffman(dictionary):
    # char - pobranie wszystkich char ze slownika
    chars = list(dictionary.keys())
    # freq - pobranie wszystkich czestosci ze slownika
    freq = list(dictionary.values())

    nodes = []

    for x in range(len(chars)):
        nodes.append(node(freq[x], chars[x]))

    while len(nodes) > 1:
        # posortuj wszystkie węzły w porządku rosnącym na podstawie ich częstotliwości
        nodes = sorted(nodes, key=lambda x: x.freq)

        # wybierz 2 najmniejsze węzły
        left = nodes[0]
        right = nodes[1]

        # przypisz wartość kierunkową do tych węzłów - lewy to 0, prawy to 1
        left.huff = 0
        right.huff = 1

        # połącz 2 najmniejsze węzły, aby utworzyć nowy węzeł jako ich rodzica
        newNode = node(left.freq + right.freq, left.symbol + right.symbol, left, right)

        # usuń 2 węzły i dodaj między innymi ich rodzica jako nowy węzeł
        nodes.remove(left)
        nodes.remove(right)
        nodes.append(newNode)

    return nodes


def readFromFile(file):
    byte_list = []
    with open(file, "rb") as f:
        while True:
            byte = f.read(1)
            if not byte:
                break
            byte_list.append(byte)
    binar = ""
    for byte in byte_list:
        int_value = ord(byte)
        binary_string = '{0:08b}'.format(int_value)
        binar += binary_string
    return binar
