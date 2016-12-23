# FreeRTOS -- GISTRE 2017

## Auteurs

* Franklin Mathieu <mathie_f@epita.fr>
* Thomas Venries <venrie_t@epita.fr>
* Wilfred Leliève <leliev_w@epita.fr>

## Fonctionnalités

Le programme possède 3 modes de fonctionnement,
chaque mode effectuant une rotation avec son prochain
en appuyant sur le bouton USER.

1. Le premier mode consiste à faire clignoter les deux LEDs
   periodiquement.

2. Le second mode consiste à allumer les LEDs quand un changement
   de vitesse angulaire est détecté (autrement dit, quand on tourne
   la carte), et de les éteindre quand la carte est stationnaire.

3. Le troisième mode consiste à allumer les LEDs quand une augmentation
   de la luminosité est détectée. À contrario, les LEDS s'éteignent quand
   la luminosité baisse ou reste identique. (nécessite un capteur
   de luminosité)

## Compilation

La compilation s'effectue avec `make`.

## Déploiement sur la stm32f429i-Discovery

Pour flasher la carte avec openocd, lancer `make deploy`.
