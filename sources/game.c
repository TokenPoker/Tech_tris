#include <stdio.h>
#include <string.h>
#include "game.h"

void clear_full_lines(Grid *grid) {
    int i = 0;

    while (i < grid->height) {
        int ligne_pleine = 1;

        // Vérifie si la ligne i est pleine
        int j = 0;
        while (j < grid->width) {
            if (grid->shape[i][j] != '1') {
                ligne_pleine = 0;
                break;
            }
            j++;
        }

        if (ligne_pleine==1) {
            // Décale toutes les lignes au-dessus de la ligne i vers le bas
            int k = i;
            while (k > 0) {
                int l = 0;
                while (l < grid->width) {
                    grid->shape[k][l] = grid->shape[k - 1][l];
                    l++;
                }
                k--;
            }

            // Vide la première ligne (ligne 0)
            int m = 0;
            while (m < grid->width) {
                grid->shape[0][m] = '0';
                m++;
            }

            // On reste sur la même ligne pour vérifier si la ligne qui est descendue est aussi pleine
        } else {
            // Si la ligne n'est pas pleine, on passe à la suivante
            i++;
        }
    }
}
