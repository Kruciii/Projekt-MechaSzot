#include <Arduino.h>
// Struktura Cup
// Reprezentuje pojedynczy kubek z unikalnym id i pozycją na podstawce
struct Cup 
{
  int id;          // unikalny numer kubka (0,1,2,...)
  int position;    // pozycja na podstawce 0-4 (co 72°)
};
// Klasa Holder
// Steruje silnikiem krokowym dla podstawki z 5 pozycjami (co 72°)
class Holder 
{
  private:
    int stepPin;               // Pin STEP podłączony do drivera silnika
    int dirPin;                // Pin DIR podłączony do drivera silnika
    int buzzerPin;             // Pin buzzer'a do sygnalizacji
    int stepsPerRevolution;    // Liczba kroków na pełen obrót (np. 200)
    int stepsFor72Degrees;     // Ile kroków potrzeba na 72° (1/5 pełnego obrotu)
    int currentPositionIndex;  // Aktualna pozycja: od 0 do 4 (0°-288°)

    // Wykonuje jeden krok silnika w zadanym kierunku
    void stepOnce(bool dir)
    {
      // Ustaw kierunek (HIGH - np. zgodnie z ruchem wskazówek, LOW - przeciwnie)
      digitalWrite(dirPin, dir ? HIGH : LOW);
      // Krótki impuls na STEP - jeden krok
      digitalWrite(stepPin, HIGH); // rozpoczęcie kroku
      delayMicroseconds(1000);      // czas trwania impulsu (trzeba dopasować do drivera)
      digitalWrite(stepPin, LOW); // zakończenie kroku
      delayMicroseconds(500);      // odstęp między krokami(prędkość)
    }
  // Sygnalizuje osiągnięcie pozycji (buzzer)
    void confirmPosition() 
    {
      Serial.print("✓ Osiągnięta pozycja: ");
      Serial.print(currentPositionIndex);
      Serial.print(" (kat: ");
      Serial.print(getCurrentAngle());
      Serial.println("°)");

      // 3 krótkie "bipnięcia" buzzerem
      for (int i = 0; i < 3; i++)  
      {
        digitalWrite(buzzerPin, HIGH);  // Włącz buzzer
        delay(120); // Czas trwania dźwięku
        digitalWrite(buzzerPin, LOW);  // Wyłącz buzzer
        delay(80); // Przerwa między dźwiękami
      }
    }
  public:
    // Konstruktor klasy Holder
    Holder(int stepPin_, int dirPin_, int buzzerPin_, int stepsPerRev_) // inicjalizacja
      : stepPin(stepPin_), //piny sterujące silnikiem
        dirPin(dirPin_), //piny sterujące silnikiem
        buzzerPin(buzzerPin_), //pin buzzera
        stepsPerRevolution(stepsPerRev_), //liczba kroków na obrót
        currentPositionIndex(0) // startujemy od pozycji 0 (0°)
  {
    stepsFor72Degrees = stepsPerRevolution/5; // 200 / 5 = 40 kroków na obrót o 72°
  }

    // Inicjalizacja pinów - wywołaj w setup()
    void begin() 
    {
      pinMode(stepPin, OUTPUT);
      pinMode(dirPin, OUTPUT);
      pinMode(buzzerPin, OUTPUT);

      digitalWrite(stepPin, LOW);
      digitalWrite(dirPin, LOW);
      digitalWrite(buzzerPin, LOW);

      Serial.println("Holder: inicjalizacja zakonczona.");
    }

    // Zwraca aktualny kąt podstawki w stopniach (0,72,144,216,288)
    int getCurrentAngle() const 
    {
      return currentPositionIndex * 72;
    }

    // Ruch do konkretnej pozycji 0–4 (co 72°)
    void moveToPosition(int targetIndex) 
    {
      // Sprawdzenie poprawności indexu
      if (targetIndex < 0 || targetIndex > 4) 
      {
        Serial.println("Blad: targetIndex poza zakresem 0-4!");
        return;
      }

      // Obliczamy różnicę pozycji (ile "skoków po 72°" trzeba zrobić)
      int diff = targetIndex - currentPositionIndex;

      // na razie proste przejście w jedną stronę / drugą.
      bool direction = (diff >= 0);              // true = w prawo, false = w lewo
      int stepsToDo = abs(diff) * stepsFor72Degrees; // każdy indeks to 72° => stepsFor72Degrees kroków

      Serial.print("Ruch z pozycji ");
      Serial.print(currentPositionIndex);
      Serial.print(" (");
      Serial.print(getCurrentAngle());
      Serial.print("°) do pozycji ");
      Serial.print(targetIndex);
      Serial.print(" (");
      Serial.print(targetIndex * 72);
      Serial.print("°), krokow: ");
      Serial.println(stepsToDo);

      // Wykonujemy zadany krok po pojedynczych krokach
      for (int i = 0; i < stepsToDo; i++) 
      {
        stepOnce(direction);
      }

      // Aktualizujemy zapamiętaną pozycję
      currentPositionIndex = targetIndex;

      // Dajemy sygnał, że pozycja została osiągnięta
      confirmPosition();
    }

    // Przejście do następnej pozycji (cyklicznie 0→1→2→3→4→0...)
    void nextPosition() 
    {
      int nextIndex = (currentPositionIndex + 1) % 5;
      moveToPosition(nextIndex);
    }

    // Ustaw podstawke tak, aby znalazła się przy danym kubku
    // Cup.position powinno być w zakresie 0–4 (pozycja na kole)
    void setOnCup(const Cup &cup) 
    {
      Serial.print("Ustawiam na kubek o id=");
      Serial.print(cup.id);
      Serial.print(" na pozycji=");
      Serial.println(cup.position);
      moveToPosition(cup.position);
    }
};
// KONFIGURACJA GLOBALNA
const int STEP_PIN=5;   // trzeba dopasować do naszego podłączenia
const int DIR_PIN=6;   // trzeba dopasować do naszego podłączenia
const int BUZZER_PIN=8;   // buzzer sygnalizujący pozycję

// Tworzymy obiekt Holder - podajemy liczbę kroków/obrót (u nas 200)
Holder holder(STEP_PIN, DIR_PIN, BUZZER_PIN, 200);

// Tablica kubków - maksymalnie np. 5 kubków
const int MAX_CUPS = 5;
Cup cups[MAX_CUPS];
int cupsCount = 0; // faktyczna liczba kubków

// Funkcje pomocnicze:

// Wczytuje z Serial liczbę kubków i ustawia pozycje
void initCupsFromUser() 
{
  Serial.println("Podaj liczbe kubkow (1-5, enter):");

  // Czekamy aż użytkownik coś wpisze
  while (Serial.available() == 0) 
  {
    // nic nie rób, czekaj
  }
  // Wczytujemy liczbę jako int
  int n = Serial.parseInt();
  if (n < 1) n = 1;
  if (n > 5) n = 5; // mamy fizycznie tylko 5 pozycji

  cupsCount = n; // ustawiamy liczbę kubków
  Serial.print("Ustawiono liczbe kubkow: ");
  Serial.println(cupsCount);

  // Prosty model: kubek i ma id=i i position=i (każdy kubek na innej pozycji od 0 do n-1)
  for (int i = 0; i < cupsCount; i++) 
  {
    cups[i].id = i;
    cups[i].position = i; // 0,1,2... => 0°,72°,144°,...
  }

  Serial.println("Kubki zainicjalizowane:");
  // Wyświetlamy konfigurację kubków (id,pozycje,kąty)
  for (int i = 0; i < cupsCount; i++) 
  {
    Serial.print("Cup id=");
    Serial.print(cups[i].id);
    Serial.print(" na pozycji=");
    Serial.print(cups[i].position);
    Serial.print(" (kat=");
    Serial.print(cups[i].position * 72);
    Serial.println("°)");
  }
}
// setup() – uruchamia się raz
void setup() 
{
  Serial.begin(9600);
  while (!Serial) 
  {
    ; // czekaj na otwarcie Serial
  }

  Serial.println("Start programu Holder + Cup");
  holder.begin();          // inicjalizacja pinów silnika i buzzera
  initCupsFromUser();      // pobranie liczby kubków od użytkownika
}
// loop() – główna pętla

//Obracamy się po kolei po kubkach i zatrzymujemy się na każdej pozycji.
void loop() 
{
  // Przejście po wszystkich kubkach zdefiniowanych przez użytkownika
  for (int i = 0; i < cupsCount; i++) 
  {
    // Obróć podstawkę, aby ustawić się na danym kubku
    holder.setOnCup(cups[i]);
    // W TYM MIEJSCU SILNIK STOI W NOWEJ POZYCJI
    // TU ZRÓB COŚ Z KUBKIEM w sensie nalej do niego, podaj go itp.
    Serial.print("Jestem przy kubku id="); //podaje id kubka
    Serial.print(cups[i].id);
    Serial.print(" na kącie "); //podaje kąt kubka
    Serial.print(holder.getCurrentAngle());
    Serial.println("°");
    // Przerwa przed przejściem do kolejnego kubka
    delay(2000);
  }
  // Po obsłużeniu wszystkich kubków powrót do pozycji 0 (home)
  Serial.println("Powrot do pozycji 0 (home)");
  holder.moveToPosition(0);
  // Krótka pauza przed kolejną rundą
  delay(5000);
}