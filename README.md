# Zeldouille

## 1. Introduction
### 1.1. Présentation du projet
Projet réalisé en 8 jours dans le cadre de notre enseignement.
### 1.2. Objectifs et ambitions
Les objectifs etaient de realiser un zelda like en 1 semaine et nos ambitions etait de realiser un jeu complet avec potion, boss, donjon, pnj et marchant
### 1.3. Public cible
Tout public
### 1.4. Plateformes et technologies utilisées
* Microsoft Visual Studio
* C++17
* SFML (Simple and Fast Multimedia Library)
## 2. Concept du jeu
### 2.1. Synopsis et univers du jeu
L'ambassadrice Zeldouille s'est perdue en chemin du château, celui ci à disparu à cause du Void.

Vous incarnez El Linké, qui part à sa recherche.
### 2.2. Genre et inspirations
Jeu d'aventure
Inspiré par Zelda et Undertale
### 2.3. Méchaniques de jeu principales
DASH = "touche shift"
ATTACK = "touche espace"
### 2.4. Système de progression
Aucun
## 3. Game Design
on a un systeme de dungeon remplie d'enemies avec des coffres cachés qui contienne des potion.
### 3.1. Personnages et protagonistes
il y a El Linké le personnage principale, Fred le marchant et AGIS l'antagoniste du jeu.
### 3.2. Environnements et niveaux
on commence dans une prairie verte pour ensuite rentrer dans le donjon corompue par le void, ce qui explique les mur detruit
### 3.3. Interface utilisateur
Menu de départ : Vous pensez accéder aux options, lancer le jeu ou le quitter (option ne marche pas pour le moment), menu pause pour quitter et mettre en pause et menu de victoire non fonctionnel.
IG : Icône interactif qui représente El Linké, barre de vie, nombre de potions.
### 3.4. Equilibrage et difficulté
le jeu a un design plutot simple a ce nivveau la les enemies sont tres simple a tuer mais le joueur peut etre rapidement depasser par un nombre elever d'enemies ce qui augmente la diff.
Le boss et pour le coup un peu plus dure avec des pattern defini.
## 4. Graphismes et Direction Artistique
on est sur du rogue 32x un design asser ancien qui peut rappeler des vieux jeux d'arcade mais avec des deplacements diagonals et des mouvement fluides pour mettre un peu de fraicheur.
### 4.1. Style visuel et inspirations
### 4.2. Modélisation et animations
Pour les animations on est sur des spriteSheet decouper en code puios animer dans le code
### 4.3. Effets visuels et shaders
on a aucun shader mais une cinematic au debut du jeu et un zoom sur la mort du joueur
## 5. Music et Sound Design
On a un sound design complet avec des 6 bruits de pas jouer aleatoirement, ainsi que 2 son d'epee, de dash et un bruit de mort du personnage.
### 5.1. Ambiance sonore et direction musicale
Pour la music du menu elle est generer par IA, et pendant la game on a un son d'amviance de donjon pour augmenter l'immersion et egalement une music de boss qui a ete tirer d'un boss d'elden ring.
### 5.2. Bruitages et effets audio
### 5.3. Intégration audio dans le moteur
## 6. Gestion de Projet et Planning
### 6.1. Méthodologie de gestion
### 6.2. Répartition des tâches et rôles
on avait pas vraiment de repartition des roles chacun prevenmais ce qu'il voulait faire et on s'organisais comme cela donc au final:
lucas c'est occuper de la map et des item domc .txt tileset etc (a ajouter)
Lorenzo a fait le boss, le joueur, les decoupe de tileset, les encapsulation, l'HUD, les menu, les musics, les dialogue box, les NPCs......
Aurel c'est occuper les cinematiques, les enemies, de faire le premier lobby, ainsi que le lore (a ajouter)
### 6.3. Planning et échéances
On avait donc 1 semaine pour realiser le projet et nous pensons que nous avons rendu un projet quasiment fini
## 7. Conclusion et Perspectives
Pour la conclusion
### 7.1. Résumé des résultats obtenus
Nous avons au final obtenue un resultat plus que satifesant pour le temps impartie
### 7.2. Améliorations futures et mise à jour
finir de corriger les bugs, rajouter du knockback lors de l'attaque d'enemies ainsi que de rajouter des gold et un marche.
### 7.3. Possibilités d'évolution du projet

test
L'ambassadrice Zeldouille s'est perdue sur le chemin de ... Vous incarnez El Linké, qui part à la recherche de Zeldouille. Mais les monstres de la grotte en ont décidé autrement.

## Entrées
Le jeux se joue au clavier.
* ZQSD : pour se déplacer dans 8 directions différentes
	* WASD : se déplacer aussi avec un clavier américain
* Espace : pour donner un coup d'épée et infliger des dégâts sur les ennemis
* E : intéragir avec les différents PNG
* H : utiliser une potion de heal

## Sprites
Joueur :

![Sprite du joueur](Projet%20Zelda-Like%202D/assets/player/player_for_md.png)

NPC :

![Sprite du marchand](Projet%20Zelda-Like%202D/assets/NPC/merchant.png)

Pattern enemy :

![Sprite du pattern enemy](Projet%20Zelda-Like%202D/assets/enemy/slime/slime_for_md.png)

Chaser Enemy :

![Sprite du chaser enemy](Projet%20Zelda-Like%202D/assets/enemy/necro/necro_for_md.png)

Boss :

![Sprite du boss](Projet%20Zelda-Like%202D/assets/enemy/boss/boss_for_md.png)

Nous avons pris nos sprites sur [itch.io](https://itch.io/).
