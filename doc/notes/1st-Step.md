
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

Il faut catégorisé ses 2 types de rayons pour que la moyenne des rayons courts permettent de donner la direction du son et les rayons long, on ne leur donne pas de direction mais servent à l'ambiance...

---

### Recherches à faire

#### Méthode de génération de points de départs

#### NEE (Next Event Estimation)