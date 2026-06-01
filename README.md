# Pong — C & Raylib

Un Pong classique en C avec la bibliothèque **Raylib**, joueur contre IA, en un seul fichier source.

---

## Table des matières

- [Aperçu](#aperçu)
- [Structure des fichiers](#structure-des-fichiers)
- [Compilation avec CMake](#compilation-avec-cmake)
- [Fonctionnement du jeu](#fonctionnement-du-jeu)
- [Architecture du code](#architecture-du-code)
- [Constantes configurables](#constantes-configurables)

---

## Aperçu

- Fenêtre 1280×720 pixels, 144 FPS
- Joueur contre IA (tracking automatique)
- Balle avec accélération à chaque rebond sur une raquette (+5%)
- Direction de service aléatoire après chaque point
- Overlay game over avec redémarrage à la demande

---

## Structure des fichiers

```
Pong/
├── src/
│   └── main.c          # Logique complète du jeu (structs, IA, rendu)
└── CMakeLists.txt
```

---

## Compilation avec CMake

### Prérequis

- CMake 3.14+
- Raylib installé sur le système

### CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.15)
project(Pong VERSION 1.0 LANGUAGES C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

find_package(raylib REQUIRED)

add_executable(Pong src/main.c)
target_link_libraries(Pong raylib)
target_compile_options(Pong PRIVATE -Wall -Wextra)
```

L'exécutable est généré directement dans le dossier `bin/` à la racine du projet.

### Compilation

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

---

## Fonctionnement du jeu

| Action | Touche |
|---|---|
| Monter | `W` |
| Descendre | `S` |
| Redémarrer après un point | `Espace` |
| Quitter | `Échap` / fermer la fenêtre |

Le joueur contrôle la raquette gauche. L'IA contrôle la raquette droite en suivant automatiquement la position verticale de la balle. Un point est marqué lorsque la balle sort de l'écran. Le score est conservé jusqu'à la fermeture du jeu.

---

## Architecture du code

### Structures

```c
Paddle  →  position (x, y), vitesse, score
Ball    →  position (x, y), vitesse (speedX, speedY), rayon
```

### Boucle de jeu

La boucle suit le schéma classique **Update → Draw** :

1. Déplacement de la balle selon `speedX` / `speedY` × `dt`
2. Rebond sur les murs haut et bas
3. Détection de collision balle/raquette avec `CheckCollisionCircleRec`
4. Accélération de la balle de 5% à chaque rebond sur une raquette
5. Détection de sortie de balle → point marqué → état `gameOver`
6. Lecture des inputs joueur (`KEY_W` / `KEY_S`)
7. Mise à jour de la position IA (tracking centre raquette vers centre balle)
8. Clamping des raquettes aux bornes de l'écran
9. Rendu : fond, ligne centrale, scores, raquettes arrondies, balle

### IA

L'IA déplace sa raquette vers le centre de la balle à chaque frame. Elle n'anticipe pas les rebonds — sa vitesse (`550.0f`) est légèrement inférieure à celle de la balle au service (`550.0f`) ce qui la rend battable.

### ResetGame

Recentre la balle et les raquettes, et tire aléatoirement la direction du service (horizontale et verticale indépendamment).

---

## Constantes configurables

Définies en haut de `main.c` :

| Constante | Valeur par défaut | Description |
|---|---|---|
| `SCREEN_WIDTH` | `1280` | Largeur de la fenêtre en pixels |
| `SCREEN_HEIGHT` | `720` | Hauteur de la fenêtre en pixels |
| `PADDLE_WIDTH` | `20.0f` | Largeur d'une raquette en pixels |
| `PADDLE_HEIGHT` | `150.0f` | Hauteur d'une raquette en pixels |
| `PADDLE_MARGIN` | `30.0f` | Distance raquette / bord de l'écran |
| `BALL_RADIUS` | `15.0f` | Rayon de la balle en pixels |
