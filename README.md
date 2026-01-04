🤖 MechaSzot – Zautomatyzowany Barman
=====================================

**MechaSzot** to projekt robota barmańskiego opartego na platformie Arduino. System automatycznie pobiera składniki, miesza je i rozlewa do kieliszków na obrotowej karuzeli. Projekt demonstruje wykorzystanie magistrali I2C, maszyny stanów (FSM) oraz obsługę przerwań w systemach wbudowanych.

📋 Spis Treści
--------------

1.  [Wstęp i Funkcjonalności](https://www.google.com/search?q=#1-wstęp-i-funkcjonalności)
    
2.  [Architektura Sprzętowa](https://www.google.com/search?q=#2-architektura-sprzętowa)
    
3.  [Konfiguracja (Config.h)](https://www.google.com/search?q=#3-konfiguracja-configh)
    
4.  [Struktura Kodu (Klasy)](https://www.google.com/search?q=#4-struktura-kodu-klasy)
    
5.  [Logika Systemu (Maszyna Stanów)](https://www.google.com/search?q=#5-logika-systemu-maszyna-stanów)
    
6.  [Wskazówki dla Studentów](https://www.google.com/search?q=#6-wskazówki-dla-studentów)
    

1\. Wstęp i Funkcjonalności
---------------------------

Głównym zadaniem robota jest przygotowanie drinków wg zadanego przepisu. Proces obejmuje:

*   **Pobieranie:** 4 pompy perystaltyczne dozują składniki z butelek.
    
*   **Mieszanie:** Składniki trafiają do głównego zbiornika, gdzie są mieszane.
    
*   **Dystrybucja:** Gotowy napój jest rozlewany do 5 kieliszków na obrotowej tacy.
    

System sterowany jest przez Arduino, wykorzystując magistralę **I2C** do komunikacji z peryferiami (ekran, ekspandery portów), co pozwala zaoszczędzić piny mikrokontrolera.

2\. Architektura Sprzętowa
--------------------------

Zrozumienie sprzętu jest kluczowe przed analizą kodu. System składa się z trzech głównych modułów wykonawczych:

1.  **Hydraulika:** Pompy i mieszadło sterowane przekaźnikami.
    
2.  **Interfejs:** Ekran LCD i przyciski sterujące menu.
    
3.  **Mechanika:** Silnik krokowy obraca tacę z kieliszkami (Holder).
    

### Magistrala I2C i Ekspandery (PCF8574)

Arduino posiada ograniczoną liczbę pinów, dlatego zastosowano układy **PCF8574**. Działają one jak "rozgałęziacze" – wykorzystując tylko dwie linie (SDA, SCL), zyskujemy dodatkowe wejścia/wyjścia.

*   **PCF #1:** Steruje przekaźnikami (Pompy, Mieszadło).
    
*   **PCF #2:** Obsługuje przyciski (Menu, Start).
    

Fragment kodu

Plain textANTLR4BashCC#CSSCoffeeScriptCMakeDartDjangoDockerEJSErlangGitGoGraphQLGroovyHTMLJavaJavaScriptJSONJSXKotlinLaTeXLessLuaMakefileMarkdownMATLABMarkupObjective-CPerlPHPPowerShell.propertiesProtocol BuffersPythonRRubySass (Sass)Sass (Scss)SchemeSQLShellSwiftSVGTSXTypeScriptWebAssemblyYAMLXML`   graph TD      MCU[Arduino] -->|I2C| PCF_RELAY[PCF8574 - Przekaźniki]      MCU -->|I2C| PCF_BTN[PCF8574 - Przyciski]      MCU -->|I2C| LCD[Ekran LCD]      MCU -->|Digital Pins| STEPPER[Silnik Krokowy]      PCF_RELAY --> Pumps[Pompy 1-4]      PCF_RELAY --> Mixer[Mieszadło]      PCF_RELAY --> PumpOut[Pompa Wylewająca]   `

3\. Konfiguracja (Config.h)
---------------------------

Plik Config.h to "centrum dowodzenia". Tutaj definiujemy fizyczne parametry robota. Zmiany w sprzęcie (np. wymiana rurek na grubsze) wymagają edycji tylko tego pliku.

### Kluczowe ustawienia:

*   **Adresy I2C:** ADDR\_RELAYS, ADDR\_BUTTONS – muszą zgadzać się z fizycznymi zworkami na modułach.
    
*   **Piny:** Przypisanie konkretnych pomp do pinów ekspandera.
    
*   **Logika kieliszków:**
    
    *   SHOT\_SIZE\_ML (np. 40ml) – ile nalać do jednego kieliszka.
        
    *   CUPS\_COUNT (np. 5) – liczba kieliszków na tacy.
        

### 🧪 Kalibracja Pomp

Pompy są sterowane czasem ("Time-based dispensing"). Nie wiedzą, ile nalały – wiedzą tylko, jak długo pracują.

Współczynnik kalibracji określa liczbę milisekund potrzebną do nalania **1 ml** płynu.

C++

Plain textANTLR4BashCC#CSSCoffeeScriptCMakeDartDjangoDockerEJSErlangGitGoGraphQLGroovyHTMLJavaJavaScriptJSONJSXKotlinLaTeXLessLuaMakefileMarkdownMATLABMarkupObjective-CPerlPHPPowerShell.propertiesProtocol BuffersPythonRRubySass (Sass)Sass (Scss)SchemeSQLShellSwiftSVGTSXTypeScriptWebAssemblyYAMLXML`// Jeśli wartość to 80.0f, pompa musi pracować 80ms, aby nalać 1ml.  #define CALIB_PUMP_1 80.0f` 

4\. Struktura Kodu (Klasy)
--------------------------

Kod został podzielony na obiekty odpowiadające fizycznym elementom robota.

### A. Klasa Relay (Włącznik)

Obsługuje piny na ekspanderze PCF8574.

*   **Active Low:** Obsługuje logikę odwrotną (stan NISKI włącza przekaźnik), co jest standardem w modułach przekaźników.
    
*   **Bezpieczeństwo:** Metoda allOff() natychmiast wyłącza wszystko.
    

### B. Klasa Pump (Inteligentny Barman)

Wrapper na przekaźnik, który przelicza objętość na czas.

*   **Nieblokowanie (update()):** Pompa **NIE** używa delay(). Zapisuje czas startu i w każdej pętli sprawdza, czy czas już minął. Dzięki temu procesor może w międzyczasie obsługiwać przyciski czy ekran.
    

### C. Klasa Holder (Karuzela)

Steruje silnikiem krokowym.

*   Oblicza liczbę kroków potrzebną do obrotu o 1/5 obwodu (72 stopnie).
    
*   Metoda moveToPosition wykonuje fizyczny ruch tacy.
    

### D. Klasa ButtonManager (Przerwania)

Najbardziej zaawansowana część. Nie pyta ciągle "czy wciśnięto?" (polling), lecz czeka na sygnał.

1.  Naciśnięcie przycisku zmienia stan pinu INT\_PIN.
    
2.  Uruchamia się szybkie przerwanie (ISR).
    
3.  Dopiero w pętli głównej procesor sprawdza przez I2C, co dokładnie wciśnięto.
    

5\. Logika Systemu: Maszyna Stanów
----------------------------------

System działa jako **Skończona Maszyna Stanów (FSM)**. W danej chwili robot może być tylko w jednym trybie.

### Diagram Przepływu (Proces tworzenia drinka)

Fragment kodu

Plain textANTLR4BashCC#CSSCoffeeScriptCMakeDartDjangoDockerEJSErlangGitGoGraphQLGroovyHTMLJavaJavaScriptJSONJSXKotlinLaTeXLessLuaMakefileMarkdownMATLABMarkupObjective-CPerlPHPPowerShell.propertiesProtocol BuffersPythonRRubySass (Sass)Sass (Scss)SchemeSQLShellSwiftSVGTSXTypeScriptWebAssemblyYAMLXML`   stateDiagram-v2      [*] --> STATE_MENU      STATE_MENU --> STATE_POURING : Wybór i Start      state "Przygotowanie (Zbiornik)" as Prep {          STATE_POURING --> STATE_MIXING : Składniki nalane          STATE_MIXING --> STATE_DISPENSING : Wymieszane      }      state "Rozlewanie (Karuzela)" as Loop {          STATE_DISPENSING --> STATE_ROTATING : Kieliszek pełny          STATE_ROTATING --> STATE_DISPENSING : Następny kieliszek      }      STATE_DISPENSING --> STATE_DONE : Wszystkie (5/5) gotowe      STATE_DONE --> STATE_MENU : Reset   `

### Opis Stanów:

1.  **STATE\_MENU:** Oczekiwanie na wybór przepisu przyciskami NEXT/PREV.
    
2.  **STATE\_POURING:** Pompy dozują składniki do mieszalnika (zgodnie z amounts\[\]).
    
3.  **STATE\_MIXING:** Mieszadło pracuje przez mixTimeMs.
    
4.  **STATE\_DISPENSING:** Pompa główna nalewa porcję (SHOT\_SIZE\_ML) do kieliszka.
    
5.  **STATE\_ROTATING:** Karuzela obraca się do następnej pozycji.
    
6.  **STATE\_DONE:** Koniec pracy, wyświetlenie komunikatu.
    

6\. Wskazówki dla Studentów
---------------------------

> ⚠️ Uwaga: Kod blokujący
> 
> Funkcja holder->nextPosition() (obrót silnika) jest funkcją blokującą. Oznacza to, że w trakcie obracania tacy interfejs może chwilowo nie reagować. Jest to zamierzone uproszczenie.

> 🔌 Hardware Check
> 
> Jeśli przyciski nie działają, sprawdź połączenie pinu INT\_PIN (zazwyczaj D2 lub D3 w Arduino Uno). Bez sygnału przerwania ButtonManager nie zadziała.

> 🛑 Emergency Stop
> 
> Przycisk BACK pełni funkcję wyłącznika bezpieczeństwa. Jego naciśnięcie wywołuje funkcję stopAll(), która natychmiast odcina zasilanie pomp i resetuje maszynę do menu.
