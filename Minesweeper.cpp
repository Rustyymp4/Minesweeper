// Le projet avec flavieng la.cpp : Ce fichier contient la fonction 'main'. L'ex�cution du programme commence et se termine � cet endroit.
//

#include <iostream>

#include <stdio.h>



/// <initialisation des variables>
/// Ici on initiallise toutes les variables universelles au programme
/// </initialisation des variables>

int i, j;
int winStatus = 0;
int loseStatus = 0;
int inputI, inputJ;
int checkI, checkJ;
int flags = 0;

#define ROUGE "\x1b[31m"
#define VERT "\x1b[32m"
#define JAUNE "\x1b[33m"
#define BLEU "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define COLOR_RESET "\x1b[0m"


#define wSize 16
#define hSize 16
#define mines 2

int remaining = hSize * wSize - mines;

const char* colors[9] =
{
	"[#]",
	BLEU "[1]" COLOR_RESET,
	VERT "[2]" COLOR_RESET,
	ROUGE "[3]" COLOR_RESET,
	MAGENTA "[4]" COLOR_RESET,
	JAUNE "[5]" COLOR_RESET,
	JAUNE "[6]" COLOR_RESET,
	JAUNE "[7]" COLOR_RESET,
	JAUNE "[8]" COLOR_RESET

};

const char* l[wSize]{ " 0]"," 1]"," 2]"," 3]"," 4]"," 5]"," 6]"," 7]"," 8]"," 9]","10]","11]","12]","13]","14]","15]" };

typedef struct Cell {
	const char* display;
	int bomb;
	int nearby;
	int isSelected;
	int isFlagged;
}Cell;



//i == ligne
//j == collone

void check(int inputI, int inputJ, struct Cell grid[16][16]) {

	///On creuse les cases d'a cote

	grid[inputI][inputJ].display = colors[grid[inputI][inputJ].nearby];


	if (grid[inputI][inputJ].bomb == 1) {
		return;
	}

	if (grid[inputI][inputJ].nearby > 0) {
		grid[inputI][inputJ].display = colors[grid[inputI][inputJ].nearby];
		grid[inputI][inputJ].isSelected = 1;
		remaining--;
		return;
	}

	if (wSize >= inputI && inputI > 0 && grid[inputI - 1][inputJ].isSelected == 0) {
		check(inputI - 1, inputJ, grid);
		if (hSize >= inputJ && inputJ > 0 && grid[inputI - 1][inputJ - 1].isSelected == 0) {
			grid[inputI][inputJ].isSelected = 1;
			remaining--;
			check(inputI - 1, inputJ - 1, grid);
		}
		if (0 <= inputJ && inputJ < hSize - 1 && grid[inputI - 1][inputJ + 1].isSelected == 0) {
			grid[inputI][inputJ].isSelected = 1;
			remaining--;
			check(inputI - 1, inputJ + 1, grid);
		}
	}

	if (hSize >= inputJ && inputJ > 0 && grid[inputI][inputJ - 1].isSelected == 0) {
		grid[inputI][inputJ].isSelected = 1;
		remaining--;
		check(inputI, inputJ - 1, grid);
		if (inputI <= wSize - 1 && grid[inputI + 1][inputJ - 1].isSelected == 0) {
			grid[inputI][inputJ].isSelected = 1;
			remaining--;
			check(inputI + 1, inputJ - 1, grid);
		}
	}

	if (0 <= inputI && inputI < wSize - 1 && grid[inputI + 1][inputJ].isSelected == 0) {
		grid[inputI][inputJ].isSelected = 1;
		remaining--;
		check(inputI + 1, inputJ, grid);
		if (0 <= inputJ && inputJ < hSize - 1 && grid[inputI + 1][inputJ + 1].isSelected == 0) {
			grid[inputI][inputJ].isSelected = 1;
			remaining--;
			check(inputI + 1, inputJ + 1, grid);
		}
	}

	if (0 <= inputJ && inputJ < hSize - 1 && grid[inputI][inputJ + 1].isSelected == 0) {
		grid[inputI][inputJ].isSelected = 1;
		remaining--;
		check(inputI, inputJ + 1, grid);
	}

}


/// <gridDIsplayInit>
/// .display = [#] par defaut, [>] quand isFlagged == 0, [ ] quand isSelected + nearby + bomb + isFlagged == 0, [*] quand winStatus ou loseStatus == 1
/// .bomb = 0 par defaut, si == 1 alors une bombe est presente, affecte .display quand winStatus ou loseStatus == 1
/// .nearby = 0 par defaut, peut etre aller de 0 a 8 selon les bombes environnantes, affecte .display quand n'est pas egal a 0
/// .isSelected = 0 par defaut, sert a ne pas pouvoir rejouer sur la meme case, sert a ne pas pouvoir avoir de bombe sur la premiere case, affecte .bombe lors de l'initiallisation
/// .isFlagged = 0 par defaut, sert a marquer une case qui va devenir injouable jusqu'a ce que le drappeau soit enleve, affecte .display lorsqu'egal a 1
/// </gridDIsplayInit>


void gridDisplayInit() {

	char choixRF;
	Cell grid[wSize][hSize];

	///Initialisation du tableau

	for (i = 0; i < wSize; i++) {
		for (j = 0; j < hSize; j++) {
			grid[i][j].display = "[ ]";
			grid[i][j].bomb = 0;
			grid[i][j].nearby = 0;
			grid[i][j].isSelected = 0;
			grid[i][j].isFlagged = 0;
		}
	}




	///Initialisation des bombes

	int i, j, count = 0;
	while (count < mines) {
		i = rand() % wSize;
		j = rand() % hSize;
		if (grid[i][j].bomb != 1) {
			grid[i][j].bomb = 1;

			///On fait les nearby des cases aux alentours

			//centre
			grid[i][j].nearby++;

			//bas droite
			if (i < 15 && j < 15) {
				grid[i + 1][j + 1].nearby++;
			}

			//droite
			if (i < 15) {
				grid[i + 1][j].nearby++;
			}

			//haut droite
			if (i < 15 && j > 0) {
				grid[i + 1][j - 1].nearby++;
			}

			//bas
			if (j < 15) {
				grid[i][j + 1].nearby++;
			}

			//haut
			if (j > 0) {
				grid[i][j - 1].nearby++;
			}

			//haut gauche
			if (j > 0 && i > 0) {
				grid[i - 1][j - 1].nearby++;
			}

			//gauche
			if (i > 0) {
				grid[i - 1][j].nearby++;
			}

			//bas gauche
			if (j < 15 && i > 0) {
				grid[i - 1][j + 1].nearby++;
			}

			count++;

		}
	}

	while (loseStatus == 0 && winStatus == 0) {

		///Print du tableau

		system("cls");
		printf(CYAN " /] 0] 1] 2] 3] 4] 5] 6] 7] 8] 9]10]11]12]13]14]15]\n" COLOR_RESET);

		for (i = 0; i < wSize; i++) {
			printf(CYAN "%s" COLOR_RESET, l[i]);
			for (j = 0; j < hSize; j++) {
				printf("%s", grid[i][j].display);
			}
			printf("\n");
		}


		printf_s(ROUGE "\n%d " COLOR_RESET, remaining);
		printf_s(JAUNE "spots left to dig \n" COLOR_RESET);
		printf_s(CYAN "%d " COLOR_RESET, mines - flags);
		printf_s(JAUNE "mines left to flag \n" COLOR_RESET);




		///Demande les coordonees de la case voulue et verifie les coordonees

		while (loseStatus == 0 && winStatus == 0)
		{
			printf("Entrez des coordones sous le format Ligne *espace* Collone : ");
			scanf_s("%d %d", &inputI, &inputJ);
			checkI = inputI;
			checkJ = inputJ;
			while (getchar() != '\n');

			if (15 >= inputI >= 0 && 0 <= inputJ <= 15) {
				break;
			}
			printf("Coordonnes invalides, veuillez reessayer (MAX 15 MIN 0)");
		}

		///Demande si on veut creuser ou mettre un drapeau

		while (loseStatus == 0 && winStatus == 0)
		{
			printf("r = Creuser, f = Mettre un Drapeau (minuscule seulement) : ");
			scanf_s("%c", &choixRF, 1);
			while (getchar() != '\n');


			//on fait un choix
			if ((choixRF == 'r' || choixRF == 'R') && grid[inputI][inputJ].isFlagged == 0 && grid[inputI][inputJ].isSelected == 0) {

				///Defaite

				if (grid[inputI][inputJ].bomb == 1) {
					system("cls");
					printf(CYAN " /] 0] 1] 2] 3] 4] 5] 6] 7] 8] 9]10]11]12]13]14]15]\n" COLOR_RESET);

					//on print le tableau quand on est mort
					for (i = 0; i < wSize; i++) {
						printf(CYAN "%s" COLOR_RESET, l[i]);
						for (j = 0; j < hSize; j++) {


							if (grid[i][j].bomb == 1) {
								grid[i][j].display = ROUGE "[*]" COLOR_RESET;
							}

							grid[inputI][inputJ].display = ROUGE "[X]" COLOR_RESET;

							if (grid[i][j].bomb == 0 && grid[i][j].isFlagged == 1) {
								grid[i][j].display = JAUNE "[X]" COLOR_RESET;
							}

							printf("%s", grid[i][j].display);
						}
						printf("\n");
					}
					printf("Bravo tu as perdu! Gros bebe");
					loseStatus = 1;
					abort;

				}



			} //Fin du if choice revealed et tout la


			///On lance la fonction de check

			if ((choixRF == 'r') || (choixRF == 'R') && grid[inputI][inputJ].isFlagged == 0 && grid[inputI][inputJ].nearby == 0) {
				check(inputI, inputJ, grid);
			}


			///On met le numero et tout

			if ((choixRF == 'r') || (choixRF == 'R') && grid[inputI][inputJ].isFlagged == 0 && grid[inputI][inputJ].nearby != 0) {
				grid[inputI][inputJ].display = colors[grid[inputI][inputJ].nearby];
			}


			///On empeche de selectionner des cases avec un drappeau

			if ((choixRF == 'r') || (choixRF == 'R') && grid[inputI][inputJ].isFlagged == 1) {
				printf("La case a ete drappeaute, tu ne peux point decouvrir cette case sans enlever le drappeau\n");

				break;
			}

			///On mets le drappeau a la case selectionnee

			if ((choixRF == 'f') || (choixRF == 'F') && grid[inputI][inputJ].isFlagged == 0) {
				grid[inputI][inputJ].isFlagged = 1;
				flags++;
				grid[inputI][inputJ].display = JAUNE "[>]" COLOR_RESET;

				break;
			}

			///On enleve le drappeau de la case selectionnee

			if (((choixRF == 'f') || (choixRF == 'F')) && grid[inputI][inputJ].isFlagged == 1) {
				grid[inputI][inputJ].isFlagged = 0;
				grid[inputI][inputJ].display = "[ ]";
				flags--;
				printf("Le drappeau a bien ete enleve\n");

				break;
			}

			//On print valeur incorrecte si l'input n'est ni R ni F
			printf("Valeur incorrecte");
			fflush(stdin);

			///Condition de victoire 

			if (remaining < 0) {
				winStatus = 1;
				printf(CYAN " /] 0] 1] 2] 3] 4] 5] 6] 7] 8] 9]10]11]12]13]14]15]\n" COLOR_RESET);

				for (i = 0; i < wSize; i++) {
					printf(CYAN "%s" COLOR_RESET, l[i]);
					for (j = 0; j < hSize; j++) {
						printf("%s", grid[i][j].display);
					}
					printf("\n");
				}
				printf("Bravo, tu as gagne!");
				winStatus = 1;
				break;

			}


		}
	}
}


/// <main>
/// Ici on lance le programme,
///		-On demande l'input
///		-On initialise la grid et pose les bombes
///		Si winStatus ou loseStatus == 0
///			-On joue un tour
///			-On check les bombes environnantes a la case selectionne et aux possibles case .nearby == 0 environnantes
///		Si winStatus == 1
///			-On print un message de victoire
///		Si loseStatus == 1
///			-On print un message de defaite
/// </main>

int main() {

	srand(time(NULL));



	while (loseStatus == 0 && winStatus == 0) {

		gridDisplayInit();
		srand(time(NULL));
		return 0;

	}

	if (winStatus == 1) {
		return 0;
	}

	if (loseStatus == 1) {
		return 0;
	}


}
