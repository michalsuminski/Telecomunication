from logging import root
from tkinter import filedialog


def encodeTextFun(file, dict):
    with open(file) as f:  # odczytaj z pliku tekst i zapisz go do "text"
        text = f.read()

    encodedText = ""  # przechowywanie zakodowanego tekstu
    pom = ""
    chars = list(dict.keys())
    encodedChars = list(dict.values())
    counter = 0

    for char in text:  # dla kazdego chara w tekscie
        for x in chars:  # znajdz jego odpowiednik w slowach kodowych
            if char == x:  # jesli zostanie znaleziony to slowo kodowe odpowiadajace znakowi
                pom = encodedChars[counter]  # zostanie przypisane do pom
                counter = 0
                break
            counter += 1
        encodedText += pom

    return encodedText


# zapisywanie zakodowanej wiadmości do pliku
def writeToFile(text):
    addition = 8 - len(text) % 8
    for i in range(addition):
        text += "0"
    stringtobytes = []  # zamiana stringa na bajty
    one_byte = ""
    j = 0
    for c in text + 'p':
        if j != 0 and j % 8 == 0:
            stringtobytes.append(int(one_byte, 2))
            one_byte = ""
            j = 0
        one_byte += c
        j += 1
    root.filename = filedialog.asksaveasfilename(filetypes=[("Plik tekstowy", "*.txt")], defaultextension="*.txt")
    if root.filename:
        with open(root.filename, "wb") as file:
            for byte in stringtobytes:
                file.write(byte.to_bytes(1, byteorder='big'))

    return addition
