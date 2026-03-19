# Smart Farm – Desktop App

**Smart Farm** est une application professionnelle destinée à la **gestion agricole**, développée en **C++ avec Qt** pour le desktop et le web, et intégrant des fonctionnalités automatisées via **Arduino**.  
Le projet facilite le suivi des stocks, des ventes, des fournisseurs, des produits et des équipements tout en permettant l’automatisation de certaines tâches agricoles.

---

## Table des matières
- [Description](#description)
- [Fonctionnalités](#fonctionnalités)
- [Technologies utilisées](#technologies-utilisées)
- [Exemple Arduino : LED Clignotante](#exemple-arduino--led-clignotante)
- [Installation](#installation)
- [Contribution](#contribution)
- [Auteur](#auteur)

---

## Description
Smart Farm combine la puissance de **Qt/C++** pour une interface desktop/web moderne et réactive avec des **interactions matérielles Arduino** pour automatiser des dispositifs agricoles simples.  
L’objectif : améliorer la productivité et la gestion quotidienne des fermes en centralisant toutes les informations et en facilitant l’automatisation.

---

## Fonctionnalités
- Gestion des **produits agricoles**  
- Suivi des **stocks** et des **ventes**  
- Gestion des **fournisseurs** et des **équipements**  
- Interface **desktop et web** intuitive  
- **Automatisation Arduino**, ex : signalisation via LED clignotante  

---

## Technologies utilisées
- **C++** – langage principal  
- **Qt Framework** – GUI desktop et web  
- **Arduino** – microcontrôleur pour automatisation  
- **Git & GitHub** – gestion de version  

---

## Exemple Arduino : LED Clignotante

### Matériel nécessaire
- Arduino Uno  
- LED  
- Résistance 220 Ω  
- Fils Dupont  

### Connexions
1. Connectez la résistance à l’une des broches de la LED.  
2. Connectez l’autre extrémité à la broche **GND** de l’Arduino.  
3. Connectez l’anode de la LED (broche longue) à la broche **13** de l’Arduino.  

### Code Arduino
```cpp
const int ledPin = 13; // Broche de la LED

void setup() {
  pinMode(ledPin, OUTPUT); // Définit la broche comme sortie
}

void loop() {
  digitalWrite(ledPin, HIGH); // Allume la LED
  delay(1000); // Attend 1 seconde
  digitalWrite(ledPin, LOW);  // Éteint la LED
  delay(1000); // Attend 1 seconde
}
