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
- 🔊 Effets sonores (si ajoutés)
- ⌨️ Contrôles au clavier

## 🎮 Commandes clavier

| Touche               | Action                              |
|----------------------|-------------------------------------|
| ← / →                | Déplacement gauche / droite         |
| ↓                    | Accélérer la descente               |
| ↑                    | Rotation de la pièce                |
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

* `main.c` : point d'entrée du programme
* `game.c` / `game.h` : logique du jeu
* `score.c` / `score.h` : gestion des scores
* `pieces.c` / `pieces.h` : définitions des pièces et mouvements
* `Makefile` : compilation automatique

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
