# NVS Projekt: Simulation des verteilten Algorithmus zur Synchronisation
Dieses Projekt simuliert den verteilten Algorithmus zur Synchronisation lokal in C++. Die einzelnen Worker (zu synchronisierende Einheiten) werden dabei durch eigene Threads dargestellt, welche in zufälligen Abständen den kritischen Abschnitt betreten wollen und diesen nach einiger Zeit wieder verlassen.

## Bedienung
Das Programm wird durch den Aufruf von `./dist_sync` auf der Kommandozeile gestartet. Bei einem Aufruf ohne Parameter startet das Progamm **standardmäßig mit 3 Worker-Threads**.

### Parameter
| Parametername | Datentyp        | Standardwert | Beschreibung |
| ------------- | --------------- | ------------ | ------------ |
|               | ganze Zahl >= 0 | 3            | Anzahl der Knoten, Standardwert: 3 |
| -h, --help    |                 |              | Gibt die Hilfe aus |
| -l, --log     |                 | false        | Aktiviert Logging nach dist_sync_log.log |
| -d, --debug   |                 | false        | Setzt Log-Level auf debug, nur mit -l verwendbar |

## Algorithmus
Wie schon zuvor erwähnt Simuliert dieses Programm den verteilten Algorithmus zur Synchronisation. Das ausschlaggebende bei diesem Algorithmus sit, dass es keinen zentralen Koordinator gibt. Die Reihenfolge wird stattdessen über folgenden Ablauf festgelegt:
1. Will der Teilnehmer in den kritischen Abschnitt eintreten, erzeugt er einen Request mit seiner aktuellen Systemzeit und sendet diesen an alle anderen Teilnehmer (inklusive sich selbst).
2. Empfängt ein Prozess eine Nachricht, dann hängt die Reaktion von seinem derzeitigen Status ab:
    * Befindet er sich _nicht im kritischen Abschnitt_ und _will diesen auch nicht betreten_: OK an Sender.
    * Befindet er sich _im kritischen Abschnitt_: Request in einer Warteschlange speichern.
    * Befindet er sich _nicht im kritischen Abschnitt, will diesen aber betreten_: Zeitstempel des Request mit jenem des eigenen Requests vergleichen. Ist der eingehende Zeitstempel kleiner, wird OK an den Sender zurückgesendet, andernfalls wird der Request in der Warteschlange gespeichert.
3. Verlässt der Teilnehmer den kritischen Abschnitt, entnimmt er alle gespeicherten Requests aus der Warteschlange und sendet ein OK an den jeweiligen Sender zurück.

## Verwendete Bibliotheken
* [CLI11](https://github.com/CLIUtils/CLI11)
* [spdlog](https://github.com/gabime/spdlog)
* [fmt](https://github.com/fmtlib/fmt)
* [magic enum](https://github.com/Neargye/magic_enum)
