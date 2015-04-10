# projet-backgammon
Projet de développement en C du jeu backgammon, comprenant une interface graphique ainsi qu'une intelligence artificielle.

##Compilation
Compilation de l'IA :

```
$ make strategy
```

Compilation de l'interface :

```
$ make interface
```

Sinon utiliser le fichier INT-Makefile

Compilation de l'ensemble :

```
$ make
```

##Dépendance
* L'interface utilse les bibliothèques SDL (vers. 2).
* De préference, utiliser un environnement Linux pour compiler.
* Sinon, le projet utilise les bibliothèques standard.

##IA
Notre IA (conçue et codée par Fibri) examine chacun des pions. Pour chaque pion, elle génére toutes les combinaisons de 
déplacement possible. Elle pondère chaque combinaison de déplacements et choisit la meilleure.

La pondération se calcule par l'avancement que la combinaison apporte.

Exemple : On a un pion en case 6 et deux dés : 3 et 1

Le mouvement 6 => 9 donne une pondération de +3.

Le mouvement 9 => 10 donne une pondération de +1.

La combinaison donne une pondération de +4.

##Interface
Pour plus d'information sur l'interface, merci de consulter README-INTERFACE.
