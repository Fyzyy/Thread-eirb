# To Do List

## Quelle liste prendre ?

- Singly-linked list ->
(+) Faible coûts mémoire
(-) Méthode d'ajout fonctionne moyennement bien avec un système FIFO
(-) Compléxité O(n) pour supprimer un thread en particulier (débordements de pile par ex) -> trouver une méthode pour réduire la suppression de thread ?

- List ->
(+) Supression d'un thread spécifique simple
(+) Méthode d'ajout fonctionne bien avec un système FIFO
(-) Elements doublement liés donc coûts mémoire plus importants

- Simple queue ->
(+) Elements simplement liés avec un pointer spécifique pour la head et pour la tail donc coûts moins élevés
(-?) On ne peut communiquer qu'avec la head pour la suppression fastidieuse et la head et la tail pour l'ajout (cohérent avec FIFO cependant)

- Tail queue ->
(-) Double pointeur pour chaque élément et deux pointeurs pour la head et la tail (haut coûts mémoire)
(+) Suppression et ajout très simple
(+) On peut traverser la liste dans les deux sens

- Single-linked Tail queue ->
(+) Faible coût mémoire
(-) Compléxité élevée
(+) En accord avec FIFO

Dilemne :
Commencer dans le dur avec des bas coûts mémoire et essayer de trouver un implémentation de nos threads qui permette de limiter la compléxité ?
Ou
Prendre un implémentation de liste simple à utiliser mais coûteuse en mémoire, quit à faire des optimisations par la suite ?

## TODO

- Regarder la complexité en temps
- Fix Valgrind 12
- Fix Valgrind 62
- Fix 71
- Fix 81
- Gérer la préemption -> attention au temps d'exécution
- Gérer les deadlocks
- Nettoyer un peu le code
- Commencer le rapport
 