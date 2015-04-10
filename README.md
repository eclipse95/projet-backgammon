# projet-backgammon
Projet de développement en C du jeu backgammon, comprenant une interface graphique ainsi qu'une intelligence artificielle.

##Compilation
Compilation de l'IA :

```
$ make strategy
```

Compilation de l'interface :

```
$ make interface (bientôt disponible)
```

Sinon utiliser le fichier INT-Makefile

Compilation de l'ensemble :

```
$ make
```

##Dépendance
* L'interface utilse les bibliothèques SDL (vers. 2). De préference, utiliser un environnement Linux pour compiler.
* Sinon, le projet utilise les bibliothèques standard.

##IA
Notre IA (conçue et codée par Fibri) examine chacun des pions. Pour chaque pion, elle génére toutes les combinaisons de 
déplacement possible. Elle pondère chaque des déplacements et choisit le déplacement "optimal".

##Interface
Pour plus d'information sur l'interface, merci de consulter README-INTERFACE.
