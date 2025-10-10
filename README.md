# Visualiseur de Graphes et Matrices (Qt/C++)

Application de bureau Qt permettant d’importer une matrice d’adjacence, de la visualiser sous forme de graphe ou de tableau, et de lancer quelques algorithmes classiques (Prim, Floyd–Warshall, Kosaraju) ainsi que la construction d’une « matrice de clusters ».

Cette application a été réalisée dans le cadre d’un projet universitaire (UE « Algorithmique avancée ») en 2025-2026, avec 4 objectifs :
- Lecture d'une matrice d'adjacence depuis un fichier texte ;
- Recherche du cheminent le plus court entre deux sommets ;
- Construction d'un arbre couvrant minimum ;
- Identification des composantes fortement connexes ;
- Création du graphe des clusters.

## Fonctionnalités

- Import d’une matrice depuis un fichier texte (.txt) ;
- Visualisation graphique interactive des sommets et arêtes :
	- Sommets déplaçables à la souris (drag & drop) ;
	- Coloration par composantes fortement connexes (Kosaraju) ;
	- Affichage des noms (ou de l’indice + 1) ;
- Visualisation tabulaire (QTableWidget) de la matrice ;
- Algorithmes intégrés :
	- Prim (arbre couvrant minimum) sur graphe non orienté pondéré ;
	- Floyd–Warshall (plus courts chemins pour tous les couples) ;
	- Kosaraju (composantes fortement connexes) ;
	- Construction d’une matrice de clusters (agrégation des CFC) ;
- Export de la vue courante (graphique ou matrice) en image.

## Formats d’entrée

Le format attendu par `Matrice(const std::string& path)` est:

1. Première ligne: la taille N de la matrice (nombre de sommets) ;
2. N lignes suivantes: N entiers par ligne, séparés par des espaces.

Exemple minimal (N = 3):

```
3
0 1 0
1 0 5
0 0 0
```

Conventions dans ce projet:

- 0 signifie « pas d’arête » entre i → j ;
- Valeur != 0: poids de l’arête (graphes orientés possibles: `data[i][j]` peut différer de `data[j][i]`) ;
- En interne, certains algos utilisent `INT64_MIN` pour « infini/aucun chemin »; ce n’est pas un format de fichier, juste un sentinelle interne. De plus, les fichiers attendent des données en `int32_t` ;
    - À noter que le fait d'utiliser de telles valeurs peut poser problème dans des cas extrêmes.

Des générateurs Python sont fournis dans `examples/` pour créer rapidement des matrices réalistes ou pondérées (voir plus bas).

## Vues disponibles (Menu > View)

- Base Graph: graphe tel que défini par la matrice ;
- Prim Graph: arbre couvrant minimum (interprète la matrice comme non orientée et ignore les zéros) ;
- Cluster Graph: graphe coloré par CFC (Kosaraju) ;
- Base Matrice: matrice telle quelle ;
- Prim Matrice: matrice de l’ACM renvoyée par Prim ;
- Floyd–Warshall: matrice des plus courts chemins (somme des poids) ; absence de chemin marquée par fond rouge et « - » ;
- Cluster Matrice: matrice d’adjacence entre clusters (CFC).

### Raccourcis depuis `mainwindow.ui`:

- Import…: `Ctrl+O` ;
- Export…: `Ctrl+S` ;
- Exit: `Ctrl+Q` ;

Pour changer de vue:

|           |  Base  |  Prim  |Floyd-Warshall|Kosaraju|
|-----------|--------|--------|--------------|--------|
|**Graph**  |`Ctrl+&`|`Ctrl+É`|   `Ctrl+"`   |`Ctrl+'`|
|**Matrice**|`Maj+&` |`Maj+É` |   `Maj+"`    |`Maj+'` |

## Utilisation

1. Lancer l’application ;
2. File > Import… et choisir un fichier `.txt` au format décrit ci-dessus ;
3. Choisir la vue dans View > … ;
4. (Vue Graph) Déplacer un sommet: clic-gauche maintenu puis glisser ;
5. Exporter une image: File > Export… (PNG/JPG/BMP).

## Construction et exécution

Pré-requis:

- Qt 6.x (Widgets), environnement MinGW ou MSVC sous Windows (testé avec Qt 6.9.3 MinGW 64-bit) ;
- Un outil de build compatible qmake ou CMake selon votre configuration (ce dépôt fournit `graphs.pro` pour qmake).

Méthodes recommandées:

- Qt Creator: ouvrir `graphs.pro`, choisir le kit, puis Build/Run ;
- Ligne de commande (exemple MinGW): générer avec qmake puis compiler. L’exécutable résultant (p. ex. `graphs.exe`) se trouvera dans un dossier `build/` de votre configuration.

Note: Un binaire peut déjà exister sous `build/…/debug/graphs.exe` selon votre environnement local. Si absent, compilez depuis les sources comme ci‑dessus.

## Génération d’exemples (Python)

Des scripts dans `examples/` aident à générer des matrices:

- `create_matrice.py`: génère une matrice binaire (0/1) avec « clusters » internes (pour un rendu plus intéressant) et quelques arêtes aléatoires externes ;
- `create_with_weights.py`: ajoute des poids aléatoires sur les arêtes d’une matrice existante ;
- `create_install_costs.py`: génère une matrice symétrique de « coûts d’installation » aléatoires, avec `[i][j] = [j][i]`.

Exemples (Windows PowerShell):

```powershell
# Créer une matrice binaire et l’écrire dans examples/size_50/matrice.txt
python .\examples\create_matrice.py

# Charger la matrice binaire, lui ajouter des poids et sauvegarder dans examples/size_50/with_weights.txt
python .\examples\create_with_weights.py

# Générer une matrice symétrique de coûts d’installation (dimension configurable dans le script)
python .\examples\create_install_costs.py
```

Les fichiers produits suivent le format d’entrée décrit plus haut et peuvent être importés via File > Import…

## Aperçu de l’architecture

- `matrice.h/.cpp`: structure de données matricielle + algorithmes (Prim, Floyd–Warshall, Kosaraju, clusterMatrice) ;
- `graphwidget.h/.cpp`: rendu du graphe (Qt Widgets). Calcul de placement par « clusters » sur un cercle et disposition interne; couleurs par CFC; interaction souris ;
- `matricewidget.h/.cpp`: rendu tabulaire avec coloration (rouge = 0/infini, vert = valeur > 0) ;
- `mainwindow.h/.cpp/.ui`: shell de l’application, menus, actions, chargement/export et sélection des vues ;
- `examples/`: générateurs Python et échantillons.

Notes d’implémentation:

- Valeur 0 = absence d’arête. Certaines opérations (Prim/Floyd–Warshall) ignorent ces entrées ;
- Prim suppose un graphe non orienté: l’arête choisie i–j est recopiée symétriquement `mst[i][j] = mst[j][i]` ;
- Kosaraju calcule les CFC sur le graphe orienté tel quel (0 = pas d’arête, >0 = arc) ;
- `clusterMatrice()` agrège les nœuds par CFC et compte les arcs inter‑clusters (`>0`) pour remplir la matrice de sortie.

## Dépannage

- Import impossible: vérifier que la première ligne contient bien N et que chaque ligne compte N entiers ;
- Graphes « disjoints » en vue Prim: l’ACM ne couvre que la composante contenant le premier sommet si le graphe n’est pas connexe ;
- Floyd–Warshall: un « - » en cellule signifie « pas de chemin » (infini); en interne on utilise `INT64_MIN` comme sentinelle, ce qui peut poser problème dans des cas extrêmes.

## Licence

Ce programme peut être librement utilisé et modifié.

La seule exception concerne l'utilisation de ce projet dans le but de réaliser un projet universitaire ou scolaire similaire, dans quel cas il est demandé d'utiliser uniquement les parties n'étant pas demandées dans ledit projet (par exemple l'interface graphique ou les générateurs d'exemples).