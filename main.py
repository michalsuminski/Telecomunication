from HuffmanTree import *
from counter import *
from encodeText import *
import pickle, socket

HOST = "127.0.0.1"
PORT = 65432

print('Copyright by Michał Sumiński & Jan Płoszaj')

print("1.receiver, 2.sender")
a = input()
if a == '1':  # odbieranie

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind((HOST, PORT))
    s.listen(1)
    conn, addr = s.accept()
    print('Connected by', conn)

    a = conn.recv(2)
    print("Supplement received")

    # zmienna przechowująca dopełnienie do pliku zakodowanego
    # jest nam to potrzebne przy odkodowaniu, aby wiedziec ile bitow pominac
    suplement = int.from_bytes(a, byteorder='big')
    print("Supplement: ")
    print(suplement)

    data = conn.recv(4096)
    received_tree = pickle.loads(data)
    conn.send(b'ConfirmTree')
    answer = conn.recv(1024)
    print("Nodes received")
    if answer == b'SendingFile':
        f = open('received_encoded.txt', 'wb')  # otworz
        while (True):
            l = conn.recv(1024)
            f.write(l)

            if not l:
                break
        f.close()
    conn.close()
    print("Printing received nodes")
    printNodes(received_tree[0])

    enc = readFromFile("received_encoded.txt")
    real = ""
    if suplement == 0:
        real = enc
    else:
        real = enc[0:-suplement]
    str = decodeHuff(received_tree[0], real)
    with open('decoded.txt', 'w') as file:
        file.write(str)

else:  # wysyłanie
    print("Pick file you want to compress")
    root.filename = filedialog.askopenfilename(filetypes=[("Plik tekstowy", "*.txt")], defaultextension="*.txt")
    dictionary = create_dictionary(root.filename)  # tworzenie słownika dla danego pliku tekstowego
    node = calculate_huffman(dictionary)  # utworzenie drzewa huffmana
    print("Created nodes")
    printNodes(node[0])
    encodedText = encodeTextFun(root.filename, encoded_dict)
    print("Pick file to save encoded text")
    add = writeToFile(encodedText)

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((HOST, PORT))

    print(s)

    s.send(add.to_bytes(2, byteorder='big'))
    print("Sending complement")
    # Zapakuj obiekt i wyślij go przez serwer
    data_string = pickle.dumps(node)
    s.send(data_string)
    answer = s.recv(1024)
    print("Sending created nodes")
    if answer == b'ConfirmTree':
        s.send(b'SendingFile')
        f = open("encoded.txt", "rb")
        l = f.read(1024)
        while (l):
            s.send(l)
            l = f.read(1024)
        f.close()
        print("Sending completed")
    s.close()