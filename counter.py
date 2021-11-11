def create_dictionary(file):
    with open(file) as f:  # odczytaj z pliku tekst i zapisz go do "text"
        text = f.read()

    counter = {}  # słownik przechowujący dane {litera : częstotliwość występowania w tekście}

    set_of_signs = set(text)
    for char in text:  # dla kazdego chara w tekscie policz liczbe jego wystepowania w tekscie
        if char in counter:
            counter[char] += 1  # zwiększ licznik zarejestrowanej literki
        else:
            counter[char] = 1  # zarejestruj nową literkę

    # posortowany słownik od najmniejszej częstotliwości do największej
    sorted_counter = {k: v for k, v in sorted(counter.items(), key=lambda item: item[1])}

    return counter
