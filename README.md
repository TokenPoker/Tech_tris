# Tech_tris 🎮

**Tech_tris** est un projet de jeu de Tetris développé en langage **C** avec la bibliothèque **SDL2**, dans le cadre d’un projet scolaire en première année d’école d’ingénieur.

## 📦 Fonctionnalités

- 🎮 Interface graphique avec SDL2
- ⬇️ Déplacement fluide des pièces
- 🔄 Rotation des pièces
- 🧱 Détection des collisions et empilement
- 💥 Suppression des lignes complètes
- 🏆 Système de score
- 💾 Sauvegarde des meilleurs scores
- 🔊 Effets sonores
- ⌨️ Contrôles au clavier

## 🎮 Commandes clavier

| Touche               | Action                              |
|----------------------|-------------------------------------|
| ← / →                | Déplacement gauche / droite         |
| ↓                    | Accélérer la descente               |
| A                    | Rotation de la pièce vers la gauche |
| S                    | Rotation de la pièce vers la droit  |
| Espace               | Chute instantanée                   |
| Échap                | Quitter le jeu                      |



## 🚀 Lancement du jeu

### 1. Pré-requis

Assurez-vous d'avoir installé :

- [SDL2](https://www.libsdl.org/)
- `gcc` ou un compilateur compatible C

Sur Ubuntu (WSL par exemple) :

```bash
sudo apt update
sudo apt install build-essential libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev
````

### 2. Cloner le dépôt

```bash
git clone https://github.com/TokenPoker/Tech_tris.git
cd Tech_tris
```

### 3. Compilation

```bash
cd sources
make
```

### 4. Exécution

Pour lancer le jeu, utilisez la commande suivante :

```bash
make run
```
## 🧠 Organisation du projet

Voici les fichiers sources et leurs rôles :

| Fichier                 | Description                                                          |
| ----------------------- | -------------------------------------------------------------------- |
| `main.c`                | Point d'entrée du programme                                          |
| `piece.c` / `piece.h`   | Définition et manipulation des pièces de Tetris                      |
| `game.c` / `game.h`     | Logique principale du jeu : grille, placement, suppression de lignes |
| `render.c` / `render.h` | Affichage du jeu à l’écran (SDL2)                                    |
| `key.c` / `key.h`       | Gestion des entrées clavier et des actions associées                 |
| `score.c` / `score.h`   | Gestion du score et des meilleurs scores (lecture/écriture fichier)  |
| `Makefile`              | Fichier de compilation automatique                                   |

📁 **Dossier `assets/`** :
Contient les fichiers nécessaires au rendu graphique et audio :

* Images des arrière-plans et des pièces
* Musique (`Tetris Theme Music.mp3`)



## 👥 Équipe
MI3-Groupe L
Ce projet a été réalisé par :
* Kevin 
* Sheryne
* Yassin

## 📌 Objectifs pédagogiques

* Approfondir la programmation en C
* Découvrir la programmation graphique avec SDL2
* Travailler en équipe sur un projet GitHub
* Appliquer les notions de structure, de compilation et de gestion de projet

## 📝 Licence

Ce projet est open-source à des fins pédagogiques.

## 🎵 Musique

Le thème musical utilisé dans **Tech_tris** est basé sur **"Korobeiniki"**, une chanson folklorique russe du domaine public, connue comme le thème classique de Tetris.

> **Fichier utilisé** : "Tetris Theme Music.mp3"  
> **Origine** : fichier trouvé sur Internet, utilisé à des fins pédagogiques et non commerciales  
> **Avertissement** : ce fichier est utilisé uniquement dans un cadre éducatif. Il pourrait être soumis à des droits d’auteur si l’enregistrement appartient à un tiers (ex. Nintendo).