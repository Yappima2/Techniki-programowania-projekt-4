
# 🎨 Dynamiczne Figury w GDI+ (Windows API)

Aplikacja graficzna w WinAPI z użyciem GDI+, generująca animowane figury (kwadraty, trójkąty, koła), które poruszają się po ekranie i są filtrowane w zależności od wybranego trybu.

---

## 🛠️ Technologie

- C++
- WinAPI (Windows.h, Messages)
- GDI+ (grafika 2D)
- Kontrolki WinAPI: Suwaki, Przyciski
- Kompilator: Visual Studio (zalecane)

---

## 🔧 Funkcje

- Generowanie losowych figur geometrycznych
- Ruch figur po określonych trajektoriach
- 5 trybów filtrowania figur ("sitko")
- Suwaki do zmiany:
  - **prędkości pojawiania się figur**
  - **prędkości ich ruchu**

---

## 🕹️ Sterowanie GUI

| Element              | Opis                                                                                     |
|----------------------|------------------------------------------------------------------------------------------|
| `Slider 1`           | Prędkość pojawiania się figur (MIN/MAX)                                                  |
| `Slider 2`           | Prędkość ruchu figur (MIN/MAX)                                                           |
| `Tryb 0`             | Wszystkie figury są przepuszczane przez filtr                                            |
| `Tryb 1`             | Po taśmie poruszają się elementy typu koło i kwadrat, kwadrat zostaje odrzucony          |
| `Tryb 2`             | Po taśmie poruszają się elementy typu koło i kwadrat, koło zostaje odrzucone             |
| `Tryb 3`             | Po taśmie poruszają się elementy typu trójkąt i kwadrat, trójkąt zostaje odrzucony       |
| `Tryb 4`             | Po taśmie poruszają się elementy typu trójkąt i kwadrat, kwadrat zostaje odrzucony       |

---

## 📦 Kompilacja

1. Upewnij się, że używasz Visual Studio z dodanym wsparciem dla GDI+.
2. Dodaj pliki:
   - `linia1.cpp` (ten plik)
   - `linia1.h` (jeśli jest używany)
   - `resource.h`, `linia1.rc` oraz ikonki (jeśli projekt ich wymaga)
3. Dołącz biblioteki:
   ```cpp
   #pragma comment(lib, "Gdiplus.lib")
   #pragma comment(lib, "Comctl32.lib")
   ```

---

## 🧠 Klasy

### `MovingShape` (klasa bazowa)
- Abstrakcyjna klasa dla poruszających się figur.
- Właściwości: `x, y, dx, dy, size, color`
- Metody: `Move()`, `Draw()`, `IsInSieveArea()`, `GetType()`

### Pochodne:
- `MovingSquare`
- `MovingCircle`
- `MovingTriangle`

---

## 📸 Podgląd działania (opis)
- Figury poruszają się po ustalonej trajektorii.
- Gdy dotrą do obszaru sitka (prostokąt w centrum sceny), niektóre zostają usunięte – zależnie od trybu.
- Graficzne tło z tunelami oraz napisem `sitko`.
![image](https://github.com/user-attachments/assets/1fee21e4-4faf-4db1-8e2a-e3283fe64f7c)

---

## 📂 Struktura projektu

```
/projekt/
├── linia1.cpp      # główny plik z logiką
├── linia1.h        # deklaracje (jeśli obecny)
├── resource.h      # identyfikatory zasobów
├── linia1.rc       # zasoby GUI
├── ikony/          # ikonki aplikacji
```

---

## 📌 Uwagi

- Program wymaga systemu Windows (brak wsparcia dla innych OS).
- Buforowanie grafiki z `Bitmap` i `Graphics::FromImage` zapewnia płynne rysowanie.

---

## 🧑‍💻 Autorzy

Piotr Pachniewski s203254 ACiR gr.3b
Mikołaj Patyk s203446 ACiR gr.3b
