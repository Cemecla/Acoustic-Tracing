# Echo Forge
## Guide de lancement
Dernière version: [Téléchager EchoForge](https://github.com/Cemecla/Echo-Forge/releases/tag/v1.0.3) 

## Installation
1. Téléchargez le fichier depuis la page de release.
2. Décompressez l'archive.
3. Lancez l'exécutable (Fichier EchoForge.exe

## Partenaires
- Mailly-Niquay Joël
- Landreville Yuta
- Caron Jacques Michel

## Description
Projet de synthèse dans le cardre du DEC en technique de l'informatique au CVM.
Logiciel de simulation sonore dans environnement en 3D.

### Utilisation
#### Menu Principal
Le menu principal permet d'accéder à différente page du programme.

#### Simulation
Il est ici possible de sélectionner une carte préconstruite pour débuter une simulation. 
- Une fois la simulation charger et prête, il est possible de déplacer un avatar sur une carte en 3D avec les touches déclarer dans le HUD.
- Plusieurs informations et commandes sur la simulations sont disponibles dans le HUD
    - La position de l'Avatar
    - Les commandes de l'Avatar
    - Désactiver le visuel des rayons
    - Désactiver le visuel des émetteurs
    - Le nombre de rayons ayant trouvé un émetteur
    - Le temps de rafrichissement du visuel des rayons
    - Le maximum de rayon possible
    - Le nombre de rayon lancé à chaque frame
    - La position, le fichier audio et la couleur de chaque émetteur
- Durant la simulation, il est possible de visualisé les rayons ayant percutter une source de son et d'entendre le résultat en temps réel.

#### Dev Mode
- Le dev mode sert pour le moment au visuellement de la carte en Free Camera pour
l'équipe de développement.
- Il était prévu plus de fonctionnalitée pour la conception de cartes, cependant le temps manque.

#### Sound Lab
La page du Sound Lab permet de tester différents sons avec Pinna, la rotation et l'élévation avec un visuel en 2D.
Il est aussi possible de créer des Registeries utilisables dans une carte 3D.

#### Option
La page d'options permet de configurer certains aspects du programme.

- Le volume du son des éléments QT
- La configuration PINA
- Taille de la fenêtre
- Resolution de certains aspects de la vue 3D
- Couleur de l'Avatar
- Couleur secondaire de l'application

Les options sont garder en mémoire par QT Settings.

## Crédits
### Libraries
- QT par QT Group
- JUCE par Raw Material Software Limited
- JSON for Modern C++ par Niels Lohmann
- Eigen C++ par The Eigen Authors

### Assets
- spritesheet_tiles.png par Kenney.nl

### Sommaire d'utilisation IA
- Recherche sommaire sur Google (Gemini)
- Documentation (Claude, Gemini, ChatGpt)
- Écrire de documentation (Claude)
- Création de tests (Claude)
- Aide debugging configuration Visual Studio Code / QT (Claude, Gemini, ChatGpt)
- Génération de carte JSON (Claude)
- Création d'un outil de création de carte JSON (Claude)
