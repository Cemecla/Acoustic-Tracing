
# Méthode pour envoyer des Rayons

### Réflexion 1

À partir de la caméra envoyer des rayons en 360 par 360. 
Il va falloir utiliser une méthode trigonométrique pour pouvoir choisir les x rayons à envoyer. Le problème c'est que pour un meilleur rendu, il serait interessant de faire en sorte que les positions de départ des rayons autour de la caméra soit uniforme. Donc, il faut trouver des méthodes pour générer des points uniformes autour d'une sphère de façon trigonométrique.

### Réflexion 2

Il serait interessant, pour des moyens de performance, de limité le nombre de rayons envoyées.
Il faut pas qu'il y ait trop de rayons envoyer d'un seul coup, la charge de calcul causerait des latences.
Donc, on pourrait se limité à 10 - 20 rayons envoyer par "frame" de sorte qu'il se cumule avec le temps et le resultat serait le même.  
Revenons sur le "frame", on parle de frame dans le sens Frame par second (FPS). Si dans notre "jeu", on hardcode le jeu à 60 FPS et que les rayons envoyées par frame sont de 10. On finirait par avoir 600 rayons en une seconde. Donc le nombre de rayons x envoyés est très exponentiel.

### Réflexion 3

Quand un rayon fait contacte avec un mur/obstacle, on devrait checker comment le rayon continue. Est-ce que on applique un rebond classique de physique qui renvoye le rayon sortant au même angle par rapport à l'obstacle de comment il est arrivée. Est-ce qu'on relance les rayons de façons aléatoire. Ou est-ce qu'on triche en assayant de diriger le rayon vers une source de son proche. Ce qui serait envisgeable, ce serait une combinaisons des 3... Donc on utiliserait les procédé de physique de base disont à 80% et le reste (20%) serait des rayons aléatoire. Et ensuite on ajoute par dessus un rayon qui se dirige directement vers une source de son (pour chaque rebond). C'est une idées encore bancale...

### Réflexion 4
Les rayons ayant parcouru une distance plus courte arrive plus tôt et plus forte et influence grandement la direction d'ou vient le son...

Les rayons ayant parcouru une distance plus longues sont utiles pour la reverbe... Ils arrivent plus tard et moins fort et viennent de "partout".

Il faut catégorisé ces 2 types de rayons pour que la moyenne des rayons courts permettent de donner la direction du son et les rayons long, on ne leurs donnent pas de direction mais servent à l'ambiance...

### Réflexion 5
Le buffer du Temps, il faudrait que les particules avancent à la vitesse du son mais simuler cette vitesse serait une perte d'efficacité. On devrait stocker toutes les rayons ayant atteint une source sonore avec la distance que celle-ci dans un "buffer" pour ensuite les comparé avec la frame actuelle et les placer dans une ligne du temps pour les classer entre son direct ou son ambiant (reverbe).
Mais
A chaque frame on envoye des rayons et on réqupère les distances parcouru et les placer dans une ligne de temps pour savoir les quels viennent l'un avant l'autre. Le problème c'est que les rayons de la frame 1 et les rayons de la frame 2 se superpose et si on se déplace ou une nouvelle source de son apparait, ça devient problèmatique. À moins de voir la densité des sons ayant atteint leurs destination sur cette ligne du temps et les zones denses sont les sons clairs mais le problème si on a plusieurs son, il y aura un mélange des sons, devrait-t-on avoir un autre buffer ? 
On pourrait générer un buffer par source de son pour rêgler le problème du mélanges des sons... Est-ce que l'on devrait ajout un buffer par son avant même de l'entendre ou pour sauver en ressources seulement si on atteint une source?
Il faudrait que le buffer garde une ligne du temps de seulement les 2 dernières secondes au moins, on ne veut pas garder tout le son depuis le début et grâce à la vitesse du son on peux aller assez vite.
Pour s'occuper du déplacement de la caméra, on pourrait donner moins d'importance au son en fonction de l'age du son, si dans la ligne du temps, il est trop dépasser il faut diminuer le volume pour privilégier les nouveaux sons plus proches. À chaque frame on réduit de 50% le poids de des sons perçu dans la frame d'avant même si le son est encore à venir dans la ligne du temps.
Quand on parle du poids sur la ligne du temps, c'est l'importance du son au moment donné, la densité.
On pourrait également modifier le pourcentage de réduction de poids en fonction de la distance parcouru par la caméra. Plus elle bouge plus le pourcentage est élevée.

### Réflexion 6
Pour augmenter la qualité. On devrait lancer les rayons normalement. Les rayons les plus courts sont les sons clairs qui définissent le son (direction etc) et les rayons qui sont plus long servent à la reverbe et l'ambiance. Mais on calcule ou est le dernier points qui est directement joignable de tout les rayons. Donc on veux savoir c'est quoi le dernier rebond qui est capable de directement rejoindre la caméra. On effectue une moyenne pondérée du dernier points de ces rayons, les rayons les plus courts sont favorisée et les rayons longs moins.
Ce poids de pondération devrait plus tôt être défini par l'énergie que possède le rayon le plus fort.
---



### Recherches à faire

#### Méthode de génération de points de départs

#### NEE (Next Event Estimation)

#### Filtre Temporel ou Néttoyage Brutal (Si on bouge, les sons doivent être readapter)

#### Moyenne Mobile Exponentielle

#### L'effet Haas dynamique