#include "projetS1.h"

/*
	Fonctions globales, utilisables partout
*/

void global(void)
{
	/* Comme cela, à chaque fois que le programme se lancera, il chargera automatiquement les fichiers en mémoire */
	
	//chargeAdherent();
	//chargeJeux();
	//chargeEmprunt();
	//chargeReservation();
	//menu();
}

Date current_date(void)
{
	Date jour_actuel;
	time_t t = time(NULL); // définition de la variable t, de type time_t
  	struct tm jour_heure_actuel = *localtime(&t); // structure stockant divers variables comme les années, les mois, le jour, l'heure précise...
  	jour_actuel.jour = jour_heure_actuel.tm_mday;
  	jour_actuel.mois = jour_heure_actuel.tm_mon + 1;
  	jour_actuel.annee = jour_heure_actuel.tm_year + 1900;
	return jour_actuel;
}

Date saisie_date(void)
{
	Date date, date_actuelle;
	date_actuelle = current_date();
	char choix = 'n';
	
	while(choix == 'n' || choix == 'N')
	{
		printf("Saisissez le jour voulu:\t");
		scanf("%d%*c", &date.jour);
		while(date.jour < 1 || date.jour > 31)
		{
			printf("Le jour se trouve entre 1 et 31. Re-saisissez le jour voulu:\t");
			scanf("%d%*c", &date.jour);
		}

		printf("Saisissez le mois voulu:\t");
		scanf("%d%*c", &date.mois);
		while(date.mois < 1 || date.mois > 12)
		{
			printf("Le mois se trouve entre 1 et 12. Re-saisissez le mois voulu:\t");
			scanf("%d%*c", &date.mois);
		}

		printf("Saisissez l'année voulue:\t");
		scanf("%d%*c", &date.annee);
		while(date.annee < 0)
		{
			printf("L'année ne peut être négative! Saisissez l'année voulu:\t");
			scanf("%d%*c", &date.annee);
		}

		if(date.annee < 100)
		{
			date.annee += 2000;
		}
		if(date_actuelle.annee - date.annee >= 1)
		{
			printf("Êtes-vous sûr de votre saisie? La date saisie est %d ans plus tôt que la date actuelle. (o/n)\n", date_actuelle.annee - date.annee);
			scanf("%c%*c", &choix);
			while(choix != 'o' && choix != 'O' && choix != 'n' && choix != 'N')
			{
				printf("Choix non reconnu. Êtes-vous sûr de votre saisie? La date saisie est %d ans plus tôt que la date actuelle. (o/n)\n", date_actuelle.annee - date.annee);
				scanf("%c%*c", &choix);
			}
		}
		else if(date_actuelle.annee - date.annee <= -1)
		{
			printf("Êtes-vous sûr de votre saisie? La date saisie est %d ans plus tard que la date actuelle. (o/n)\n", -(date_actuelle.annee - date.annee));
			scanf("%c%*c", &choix);
			while(choix != 'o' && choix != 'O' && choix != 'n' && choix != 'N')
			{
				printf("Choix non reconnu. Êtes-vous sûr de votre saisie? La date saisie est %d ans plus tôt que la date actuelle. (o/n)\n", -(date_actuelle.annee - date.annee));
				scanf("%c%*c", &choix);
			}
		}
		else choix = 'o';
	}
	return date;
}

void fget(char *str , int max_saisie, FILE *flot, char char_arret)
{
	/*
		Nom:		fget
		Finalité: 	Retourne par adresse un str contenant un maximum de caractères définit pris d'un fichier, ou jusqu'à ce qu'il rencontre un certain caractère d'arrêt.
		Description générale:
			initialise le str passé en paramètre à '\0', pour éviter d'y avoir n'importe quoi
			tant que le nombre d'itérations est strictement inférieur au nombre max_saisie - 1 ( -1 pour garder une place pour le \0) il est fait:
				stocke dans l'index de str, correspondant au nombre d'itération, un charactère pris depuis le fichier pointé par flot
				teste si jamais le charactère pris équivaut à celui qui doit arrêter la fonction
					si oui la boucle while se brise
				on incrémente i de 1, pour converger vers max_saisie - 1 dans le test, et stocker le prochain caractère
			on ajoute à la fin du str complété un '\0'


		Variables:
		str			chaine de caractères qui servira de conteneur pour la fonction
		max_saisie	nombre entier maximal de charactères lu par la fonction
		flot		pointeur vers le fichier d'où est pris les informations de l'adhérent
		char_arret	charactère obligeant l'arrêt de la lecture par la fonction lors de son encontre
		i			variable d'incrémentation pour le test de la boucle for
	*/

	int i = 0;
	str[0] = '\0';
	for(i = 0; i < max_saisie - 1; i++) // le choix est fait ici de mettre a -1, pour que à l'appel de la fonction, on appelle directement avec le nombre de place de str
	{
		str[i] = (char)fgetc(flot);
		str[i + 1] = '\0';
		if(str[i] == char_arret)
		{
			break;
		}
	}
	str[i] = '\0';
}

/*
	Fonction de traitement du fichier adherent.don
*/

Adherent** decaleADroite_Adherent(Adherent** tAdherent, int taille_logique, int pos_insert)
{
	int i;
	for(i = taille_logique; i > pos_insert; i--)
	{
		tAdherent[i] = tAdherent[i - 1];
	}
	return tAdherent;
}

Adherent** decaleAGauche_Adherent(Adherent** tAdherent, int taille_logique, int pos_insert)
{
	int i;
	for(i = pos_insert; i < taille_logique; i++)
	{
		tAdherent[i] = tAdherent[i + 1];
	}
	return tAdherent;
}

int recherchDich_Adhe(Adherent **tAdherent, int taille_logique, int *trouve, int idAdherent)
{
	int inf = 0, middle, sup = taille_logique - 1;
	*trouve = 0;
	while(inf <= sup)
	{
		middle = (inf + sup) / 2;
		if(idAdherent <= tAdherent[middle]->idAdherent)
			sup = middle - 1;
		else inf = middle + 1;
		if(idAdherent == tAdherent[middle]->idAdherent)
			*trouve = 1;
	}
	return inf;
}

Adherent chargeAdherent(FILE *flot)
{
	/*
		Nom:		chargeAdherent
		Finalité:	Charger une ligne du fichier pointé par *flot dans une structure Adherent.

		Description générale:
			prends les informations d'une ligne du fichier sur lequel pointe le flot, et le met dans les champs correspondant de a
			fget permettra de prendre des noms et prénoms à espace (rares mais possibles)
			retourne la variable contenant l'adherent

		Variables:
			flot	pointeur sur un fichier de données
			a		variable rendu par la fonction, de type Adherent et servant de conteneur pour les informations du fichier
	*/

	Adherent a;
	fscanf(flot, "%d%*c%s%*c", &a.idAdherent, a.civilite);
	fget(a.nom , 22, flot, '\t');
	fget(a.prenom , 22, flot, '\t');
	fscanf(flot, "%d/%d/%d", &a.date_inscrip.jour, &a.date_inscrip.mois, &a.date_inscrip.annee);
	return a;
}

Adherent** chargTAdherent( Adherent* tAdherent[], int *taille_logique, int *taille_physique)
{
	/*
		Nom:		chargTAdherent
		Finalité:	charger entièrement le fichier adherent.don dans le tableau de pointeurs tAdherent, ligne par ligne. Retourne la taille logique de tAdherent,
					et par adresse tAdherent.

		Description générale:
			renvoit par adresse la taille logique du tableau

		Variables:
			tAdherent			tableau de pointeurs d'Adherent
			taille_physique		taille maximale physique du tableau tAdherent
			fichier_adherent	pointeur ouvrant le fichier adherent.don
			nouv_adhe			variable intermédiaire pour charger les données du fichier don , et les transmettre ensuite vers le tableau
			tNouvAdherent		tableau de pointeur, servant au realloc du tableau principal, pour augmenter sa taille
			taille_logique		nombre d'élément dans le tableau tAdherent, renvoyé par return
	*/

	FILE *fichier_adherent;
	Adherent nouv_adhe, **tNouvAdherent;
	fichier_adherent = fopen("adherent.don", "r");
	if(fichier_adherent == NULL)
	{
		printf("Erreur lors de l'ouverture du fichier\n");
		return NULL;
	}
	nouv_adhe = chargeAdherent(fichier_adherent);
	while(!feof(fichier_adherent))
	{
		if(*taille_logique == *taille_physique)
		{
			printf("Le tableau est trop petit, ajout de 5 espaces\n");
			tNouvAdherent = (Adherent**) realloc (tAdherent, (*taille_physique + 5) * sizeof(Adherent*));

			if(tNouvAdherent == NULL)
			{
				printf("Problème lors du realloc, la mémoire n'a pû être allouée.\n");
				return NULL;
			}
			else
			{
				tAdherent = tNouvAdherent;
				*taille_physique += 5; // on ne prends en compte le changement de taille physique que si le realloc à marché, pour garder une taille physique réelle
			}
		}
		tAdherent[*taille_logique] = (Adherent*) calloc (1, sizeof(Adherent));
		if(tAdherent[*taille_logique] == NULL)
		{
			printf("Problème de calloc, la mémoire n'a pas été allouée.\n");
			return NULL;
		}
		*tAdherent[*taille_logique] = nouv_adhe;
		(*taille_logique) ++;
		nouv_adhe = chargeAdherent(fichier_adherent);
	}
	fclose(fichier_adherent);
	return tAdherent;
}

void afficheTAdherent(Adherent* tAdherent[], int taille_logique)
{
	/*
		Nom:		afficheTAdherent
		Finalité:	afficher le contenu des pointeurs du tableau tAdherent

		Description générale:
			affiche la présentation des données
			tant qu'il n'a pas répété 
			affiche tout les adhérents du tableau tAdherent

		Variables:
			tAdherent			tableau de pointeurs d'Adherent
			taille_logique		nombre d'éléments du tableau tAdherent
			i					variable d'incrémentation pour le test de la boucle for
	*/

	int i;
	printf("idAdherent\tcivilité\tnom\tprénom\tdate d'inscription (JJ/MM/YYYY)\n");
	for(i = 0; i < taille_logique; i++)
	{
		printf("%03d\t%s\t", tAdherent[i]->idAdherent, tAdherent[i]->civilite);
		printf("%s\t%s\t", tAdherent[i]->nom, tAdherent[i]->prenom);
		printf("%02d/%02d/%02d\n", tAdherent[i]->date_inscrip.jour, tAdherent[i]->date_inscrip.mois, tAdherent[i]->date_inscrip.annee);
	}
}

Adherent** ajoutAdherent(Adherent* tAdherent[], int *taille_logique, int *taille_physique)
{
	/*
		Nom:		ajoutAdherent
		Finalité:	Insérer un adhérent dans le tableau trié tAdherent et dans le fichier adherent.don

		Description générale:
			renvoit par adresse la taille logique du tableau

		Variables:
			tAdherent			tableau de pointeurs d'Adherent
			taille_logique		nombre d'éléments du tableau tAdherent
			taille_physique		variable d'incrémentation pour le test de la boucle for
	*/

	FILE *fichier_adherent;
	Adherent a, **tNouvAdherent;
	int pos_insert = 0, trouve = 0;
	char validation = 'o', choix;

	// Choix de la position de l'adhérent
	printf("Saisissez d'abord un identifiant pour votre nouvel adhérent:\t");
	scanf("%d%*c", &a.idAdherent); // le choix est libre de faire une saisie de l'identifiant nous mêmes, car nous ne savons si les id sont données dans l'ordre ou non

	pos_insert = recherchDich_Adhe(tAdherent, *taille_logique, &trouve, a.idAdherent);
	while(trouve == 1)
	{
		printf("L'identifiant saisi existe déjà. Veuillez en sélectionner un nouveau:\t");
		scanf("%d%*c", &a.idAdherent);
		pos_insert = recherchDich_Adhe(tAdherent, *taille_logique, &trouve, a.idAdherent);
	}
	while(validation != 'n' && validation != 'N')
	{
		if(validation == 'q' || validation == 'Q')
		{
			printf("Annulation de la saisie d'un nouvel adherent.\n");
			return tAdherent;
		}

		printf("Quelle est la civilité (Mr / Mme) du nouvel adherent:\n");
		scanf("%s%*c", a.civilite);
		while(strcmp(a.civilite, "Mr") != 0)
		{
			if(strcmp(a.civilite, "Mme") == 0)
				break;
			printf("Civilité non valide, quelle est la civilité (Mr / Mme) du nouvel adherent:\n");
			scanf("%s%*c", a.civilite);
		}
		printf("Quel est le nom du nouvel ahdherent?\n");
		fgets(a.nom, 20, stdin);
		a.nom[strlen(a.nom) - 1] = '\0';
		printf("Quel est le prénom du nouvel ahdherent?\n");
		fgets(a.prenom, 20, stdin);
		a.prenom[strlen(a.prenom) - 1] = '\0';
		printf("Voici les informations de l'adherent %d:\n", a.idAdherent);
		printf("%s\t%s\t%s\n", a.civilite, a.nom, a.prenom);
		printf("Voulez vous ressaisir les informations? (n/N pour valider la saisie, q pour annuler)\n");
		scanf("%c%*c", &validation);
	}

	printf("Voulez-vous une date d'inscription automatique? (o/n)");
	scanf("%c%*c", &choix);
	if(choix == 'o' || choix == 'O')
		a.date_inscrip = current_date();
	else a.date_inscrip = saisie_date();


	if(*taille_logique == *taille_physique)
	{
		printf("Le tableau est trop petit, ajout d'un espace mémoire\n");
		printf("%d\n", tAdherent[*taille_logique - 1]->idAdherent);
		tNouvAdherent = (Adherent**) realloc (tAdherent, (*taille_physique + 1) * sizeof(Adherent*));
		if(tNouvAdherent == NULL)
		{
			printf("Problème lors du realloc, la mémoire n'a pû être allouée.\n");
			return NULL;
		}
		else
		{
			tAdherent = tNouvAdherent;
			(*taille_physique) ++; // on ne prends en compte le changement de taille physique que si le realloc à marché, pour garder une taille physique réelle
		}
	}
	tAdherent = decaleADroite_Adherent(tAdherent, *taille_logique, pos_insert);

	tAdherent[pos_insert] = (Adherent*) calloc (1, sizeof(Adherent));
	if(tAdherent[pos_insert] == NULL)
	{
		printf("Problème de calloc");
		return NULL;
	}

	*tAdherent[pos_insert] = a;
	(*taille_logique)++;
	return tAdherent;
}

void EnregistrerTAdherent(Adherent** tAdherent, int taille_logique)
{
	FILE *fichier_adherent;
	int i;
	fichier_adherent = fopen("adherent.don", "w");
	if(fichier_adherent == NULL)
	{
		printf("Problème lors de l'ouverture du fichier en mode écriture");
		return;
	}
	for(i = 0; i < taille_logique; i++)
	{
		fprintf(fichier_adherent, "%d\t%s\t", tAdherent[i]->idAdherent, tAdherent[i]->civilite);
		fprintf(fichier_adherent, "%s\t%s\t", tAdherent[i]->nom, tAdherent[i]->prenom);
		fprintf(fichier_adherent, "%02d/%02d/%02d\n", tAdherent[i]->date_inscrip.jour, tAdherent[i]->date_inscrip.mois, tAdherent[i]->date_inscrip.annee);
	}
	fprintf(fichier_adherent, "\n");
	fclose(fichier_adherent);
	printf("L'enregistrement s'est bien déroulé sans soucis.\n");
}

Adherent** modifAdherent(Adherent** tAdherent, int taille_logique)
{
	int pos_modif = 0, trouve = 0, idAdhe;
	char choix, choix_date;
	printf("Saisissez d'abord un identifiant pour l'adhérent modifié:\t");
	scanf("%d%*c", &idAdhe); // le choix est libre de faire une saisie de l'identifiant nous mêmes, car nous ne savons si les id sont données dans l'ordre ou non

	pos_modif = recherchDich_Adhe(tAdherent, taille_logique, &trouve, idAdhe);
	while(trouve == 0)
	{
		printf("L'identifiant saisi n'existe pas. Veuillez en sélectionner un nouveau:\t");
		scanf("%d%*c", &idAdhe);
		pos_modif = recherchDich_Adhe(tAdherent, taille_logique, &trouve, idAdhe);
	}
	printf("Voici les informations actuelles de l'adherent sélectionné:\n");
	printf("%03d\t%s\t", tAdherent[pos_modif]->idAdherent, tAdherent[pos_modif]->civilite);
	printf("%s\t%s\t", tAdherent[pos_modif]->nom, tAdherent[pos_modif]->prenom);
	printf("%02d/%02d/%02d\n", tAdherent[pos_modif]->date_inscrip.jour, tAdherent[pos_modif]->date_inscrip.mois, tAdherent[pos_modif]->date_inscrip.annee);
	printf("Voulez-vous modifier les informations? (o/n)\t");
	scanf("%c%*c", &choix);
	while(choix != 'o' && choix != 'O' && choix != 'n' && choix != 'N')
	{
		printf("Choix non reconnu. Voulez-vous modifier les informations? (o/n)\t");
		scanf("%c%*c", &choix);
	}
	if(choix == 'o' || choix == 'O')
	{
		choix = 'n';
		while(choix == 'n' || choix == 'N')
		{
			printf("Quelle est la civilité (Mr / Mme) de l'adhérent modifié:\n");
			scanf("%s%*c", tAdherent[pos_modif]->civilite);
			while(strcmp(tAdherent[pos_modif]->civilite, "Mr") != 0)
			{
				if(strcmp(tAdherent[pos_modif]->civilite, "Mme") == 0)
					break;
				printf("Civilité non valide, quelle est la civilité (Mr / Mme) de l'ahdherent modifié:\n");
				scanf("%s%*c", tAdherent[pos_modif]->civilite);
			}
			printf("Quel est le nom de l'adherent modifié?\n");
			fgets(tAdherent[pos_modif]->nom, 20, stdin);
			tAdherent[pos_modif]->nom[strlen(tAdherent[pos_modif]->nom) - 1] = '\0';
			printf("Quel est le prénom de l'adherent modifié?\n");
			fgets(tAdherent[pos_modif]->prenom, 20, stdin);
			tAdherent[pos_modif]->prenom[strlen(tAdherent[pos_modif]->prenom) - 1] = '\0';
			printf("Voici les informations de l'adhérent %d:\n", tAdherent[pos_modif]->idAdherent);
			printf("%s\t%s\t%s\n", tAdherent[pos_modif]->civilite, tAdherent[pos_modif]->nom, tAdherent[pos_modif]->prenom);
			printf("Validez-vous les modifications apportées jusque présent? (o/n)\t");
			scanf("%c%*c", &choix);
			while(choix != 'o' && choix != 'O' && choix != 'n' &&choix != 'N')
			{
				printf("Choix non reconnu. Voulez-vous modifier les informations? (o/n)\t");
				scanf("%c%*c", &choix);
			}
		}

		printf("Voulez-vous une date d'inscription automatique? (o/n)");
		scanf("%c%*c", &choix_date);
		if(choix_date == 'o' || choix_date == 'O')
			tAdherent[pos_modif]->date_inscrip = current_date();
		else tAdherent[pos_modif]->date_inscrip = saisie_date();
		printf("Voici les informations actuelles de l'adherent sélectionné:\n");
		printf("%03d\t%s\t", tAdherent[pos_modif]->idAdherent, tAdherent[pos_modif]->civilite);
		printf("%s\t%s\t", tAdherent[pos_modif]->nom, tAdherent[pos_modif]->prenom);
		printf("%02d/%02d/%02d\n", tAdherent[pos_modif]->date_inscrip.jour, tAdherent[pos_modif]->date_inscrip.mois, tAdherent[pos_modif]->date_inscrip.annee);
	}
	else printf("Modification annulée.");
	return tAdherent;
}

Adherent** supressAdherent(Adherent** tAdherent, int *taille_logique)
{
	int idAdhe, pos_supress, trouve;
	char choix_modif = 'n';
	while(choix_modif == 'n' || choix_modif == 'N')
	{
		printf("Quel numéro d'adherent voulez vous supprimer?");
		scanf("%d%*c", &idAdhe);

		pos_supress = recherchDich_Adhe(tAdherent, *taille_logique, &trouve, idAdhe);
		while(trouve == 0)
		{
			printf("Numéro non reconnu. Quel numéro d'adherent voulez vous supprimer?");
			scanf("%d%*c", &idAdhe);
			pos_supress = recherchDich_Adhe(tAdherent, *taille_logique, &trouve, idAdhe);
		}
		printf("Les informations de l'adherent à supprimer sont les suivantes:\n");
		printf("%03d\t%s\t", tAdherent[pos_supress]->idAdherent, tAdherent[pos_supress]->civilite);
		printf("%s\t%s\t", tAdherent[pos_supress]->nom, tAdherent[pos_supress]->prenom);
		printf("%02d/%02d/%02d\n", tAdherent[pos_supress]->date_inscrip.jour, tAdherent[pos_supress]->date_inscrip.mois, tAdherent[pos_supress]->date_inscrip.annee);
		printf("Est-ce que l'adhérent sélectionné est bien celui que vous souhaitez supprimer? (o/n, q pour quitter)");
		scanf("%c%*c", &choix_modif);
		while(choix_modif != 'o' && choix_modif != 'O' && choix_modif != 'n' && choix_modif != 'N' && choix_modif != 'q' && choix_modif != 'Q')
		{
			printf("Choix non reconnu. Voulez-vous modifier les informations? (o/n)\t");
			scanf("%c%*c", &choix_modif);
		}
		if(choix_modif == 'q' || choix_modif == 'Q')
		{
			printf("Suppression annulée");
			return tAdherent;
		}
	}
	tAdherent = decaleAGauche_Adherent(tAdherent, *taille_logique, pos_supress);
	(*taille_logique) --;
	return tAdherent;
}

Jeux chargeJeux(FILE *flot)
{
	/*
		Nom:		chargeJeux
		Finalité:	Charger une ligne du fichier pointé par *flot dans une structure Jeux.

		Description générale:
			prends les informations d'une ligne du fichier sur lequel pointe le flot, et le met dans les champs correspondant de j
			retourne la variable contenant le jeu

		Variables:
			flot	pointeur sur un fichier de données
			j		variable rendu par la fonction, de type Jeux et servant de conteneur pour les informations du fichier
	*/

	Jeux j;
	fscanf(flot, "%d%*c", &j.idJeux);
	fget(j.nom , 20, flot, '\t');
	fget(j.type , 15, flot, '\t');
	fscanf(flot, "%d", &j.quantite);
	return j;
}

Jeux* chargeTJeux(Jeux tJeux[], int *nbJeux, int *tMax)
{
	/*
		Nom:		chargeTJeux
		Finalité:	charger entièrement le fichier jeu.don dans le tableau tJeux, ligne par ligne. Retourne le tableau tJeux.

		Description générale:
			Renvoit le tableau tJeux alloué dynamiquement en mémoire.

		Variables:
			tJeux				tableau des Jeux disponibles.
			tMax				taille maximale physique du tableau tJeux
			jeu_fichier			pointeur ouvrant le fichier jeu.don
			j					variable intermédiaire pour charger les données du fichier don , et les transmettre ensuite vers le tableau
			tNouvJeux			tableau temporaire servant au realloc du tableau principal, pour augmenter sa taille
			nbJeux				nombre d'élément dans le tableau tJeux
	*/
	Jeux *tNouvJeux, j;
	FILE *jeu_fichier;
	jeu_fichier = fopen("jeu.don", "r");
	if (jeu_fichier == NULL)
	{
		printf("Problème lors de l'ouverture du fichier\n");
		return NULL;
	}
	*tMax = 5;
	tJeux = (Jeux *) malloc(*tMax * sizeof(Jeux));
	if (tJeux == NULL)
	{
		printf("Problème malloc\n");
		return NULL;
	}
	j = chargeJeux(jeu_fichier);
	while (!feof(jeu_fichier))
	{
		if (*nbJeux == *tMax)
		{
			*tMax = *tMax + 5;
			tNouvJeux = ((Jeux*) realloc (tJeux, *tMax *sizeof(Jeux)));
			if (tNouvJeux == NULL)
			{
				printf("Problème lors du realloc\n");
				return NULL;
			}
			tJeux = tNouvJeux;
		}
		tJeux[*nbJeux] = j;
		*nbJeux = *nbJeux + 1;
		j = chargeJeux(jeu_fichier);
	}
	fclose(jeu_fichier);
	return tJeux;
}

void afficheTJeux(Jeux tJeux[], int nbJeux)
{
	/*
		Nom:		afficheTJeux
		Finalité:	afficher le contenu du tableau tJeux triée par type de Jeux, puis par ordre alphabétique de leur nom

		Description générale:
			Affiche le contenu du tableau tJeux

		Variables:
			tJeux				tableau tJeux
			nbJeux				nombre d'éléments du tableau tJeux
			i					variable d'incrémentation pour le test de la boucle for
			cmp					Variable résultat d'une comparaison entre deux chaînes.
			pge					Plus grand nom alphabétiquement du tableau
			i					variable d'incrémentation pour le test de la boucle for
	*/

	//Tri du tableau par ordre alphabétique
	int pge, i, cmp;
	int n = nbJeux;
	while (n > 1)
	{
		pge = plusGrand(tJeux, n);
		echange(tJeux, pge, n-1);
		n = n-1;
	}

	//Logique
	printf("\nJeux de logique :\n\n");
	for(i = 0; i <= nbJeux-1; i++)
	{
		cmp = strcmp(tJeux[i].type, "Logique");
		if (cmp==0)
			printf("%03d\t%s\t%d restant(s)\n", tJeux[i].idJeux, tJeux[i].nom, tJeux[i].quantite);
	}

	//Plateau
	printf("\nJeux de plateau :\n\n");
	for(i = 0; i <= nbJeux-1; i++)
	{
		cmp = strcmp(tJeux[i].type, "Plateau");
		if (cmp==0)
			printf("%03d\t%s\t%d restant(s)\n", tJeux[i].idJeux, tJeux[i].nom, tJeux[i].quantite);
	}

	//Cartes
	printf("\nJeux de cartes :\n\n");
	for(i = 0; i <= nbJeux-1; i++)
	{
		cmp = strcmp(tJeux[i].type, "Cartes");
		if (cmp==0)
			printf("%03d\t%s\t%d restant(s)\n", tJeux[i].idJeux, tJeux[i].nom, tJeux[i].quantite);
	}

	//Construction
	printf("\nJeux de construction :\n\n");
	for(i = 0; i <= nbJeux-1; i++)
	{
		cmp = strcmp(tJeux[i].type, "Construction");
		if (cmp==0)
			printf("%03d\t%s\t%d restant(s)\n", tJeux[i].idJeux, tJeux[i].nom, tJeux[i].quantite);
	}

	//Tuiles
	printf("\nJeux de tuiles :\n\n");
	for(i = 0; i <= nbJeux-1; i++)
	{
		cmp = strcmp(tJeux[i].type, "Tuiles");
		if (cmp==0)
			printf("%03d\t%s\t%d restant(s)\n", tJeux[i].idJeux, tJeux[i].nom, tJeux[i].quantite);
	}
	
}

int plusGrand(Jeux tJeux[], int n)
{
	/*
		Nom:		plusGrand
		Finalité:	Compare  les noms des jeux entre-eux pour obtenir le plus grand alphabétiquement

		Description générale:
			Renvoie le plus grand nom alphabétiquement

		Variables:
			cmp					Variable résultat d'une comparaison entre deux chaînes.
			pge					Plus grand nom alphabétiquement du tableau
			i					variable d'incrémentation pour le test de la boucle for
	*/

	int i, pge = 0, cmp;
	for (i=1;i<n;i++)
	{
		cmp = strcmp (tJeux[i].nom, tJeux[pge].nom);
		if (cmp > 0)
			pge = i;
	}
	return pge;
}

void echange(Jeux tJeux[], int i, int j)
{
	/*
		Nom:		echange
		Finalité:	Echange les lignes du tableau pour les triées.

		Description générale:
			Echange les lignes du tableau pour les triées.

		Variables:
			auxID, auxQT		Variables int temporaires de l'ID et de la quantité
			auxNOM, auxTYPE		Variables char temporaires du nom et du type
	*/
	int auxID, auxQT;
	char auxNOM[20], auxTYPE[15];
	auxID = tJeux[i].idJeux;
	strcpy(auxNOM, tJeux[i].nom);
	strcpy(auxTYPE, tJeux[i].type);
	auxQT = tJeux[i].quantite;
	tJeux[i].idJeux = tJeux[j].idJeux;
	strcpy(tJeux[i].nom, tJeux[j].nom);
	strcpy(tJeux[i].type, tJeux[j].type);
	tJeux[i].quantite = tJeux[j].quantite;
	tJeux[j].idJeux = auxID;
	strcpy(tJeux[j].nom, auxNOM);
	strcpy(tJeux[j].type, auxTYPE);
	tJeux[j].quantite = auxQT;
}

Jeux *ajouterJeux(Jeux tJeux[], int *nbJeux, int *tMax)
{
	/*
		Nom:		ajouterJeux
		Finalité:	 Ajoute un jeu à tJeux et à la mémoire

		Description générale:
			Ajoute un jeu à tJeux et à la mémoire
			
		Variables:


	*/
	Jeux *tNouvJeux, j;
	int rep, i, cmp;
	printf("Nom du jeu :\n ");
	fgets(j.nom,20,stdin);
	j.nom[strlen(j.nom)-1] = '\0';
	printf("Nom lu : %s\n", j.nom);
	for(i = 0; i <= *nbJeux; i++)
	{
		cmp = strcmp(tJeux[i].nom, j.nom);
		if (cmp==0)
		{
			printf("Ce jeu existe déjà.\n");
			return NULL;
		}
	}
	printf("Nom lu : %s\n", j.nom);
	printf("Type :\n ");
	fgets(j.type,20,stdin);
	j.type[strlen(j.type)-1] = '\0';
	for(i = 0; i <= *nbJeux; i++)
	{
		cmp = strcmp(tJeux[i].type, j.type);
		if (cmp == 5)
		{
			printf("Cette catégorie n'existe pas.\n");
			return NULL;
		}
	}
	printf("Type lu : %s\n", j.type);
	printf("ID du jeu :\n");
	scanf(" %d%*c",&j.idJeux);
	for(i = 0; i <= *nbJeux; i++)
	{
		if (tJeux[i].idJeux == j.idJeux)
		{
			printf("Cet ID existe déjà.\n");
			return NULL;
		}
	}
	printf("Quantité :\n");
	scanf(" %d%*c",&j.quantite);
	if (tJeux == NULL)
	{
		printf("Problème malloc\n");
		return NULL;
	}
	if (*nbJeux == *tMax)
	{
		*tMax = *tMax + 5;
		tNouvJeux = ((Jeux*) realloc (tJeux, *tMax *sizeof(Jeux)));
		if (tNouvJeux == NULL)
		{
			printf("Problème lors du realloc\n");
			return NULL;
		}
		tJeux = tNouvJeux;
	}
	tJeux[*nbJeux] = j;
	*nbJeux = *nbJeux + 1;
	FILE *jeu_fichier;
	jeu_fichier = fopen("jeu.don","w");

	for(i = 0; i <= *nbJeux-1; i++)
	{
		fprintf(jeu_fichier, "%d\t%s\t%s\t%d\n", tJeux[i].idJeux, tJeux[i].nom, tJeux[i].type, tJeux[i].quantite);
	}
	fclose(jeu_fichier);

	return tJeux;

}

Jeux *supprimerJeux(Jeux tJeux[], int *nbJeux, int *tMax)
{
	/*
		Nom:		supprimerJeux
		Finalité:	 Supprime un jeu du tableau tJeux et de la mémoire

		Description générale:
			Supprime un jeu du tableau tJeux et de la mémoire

		Variables:


	*/
	Jeux *tNouvJeux, j;
	int rep, i, cmp;
	printf("ID du jeu à supprimer :\n ");
	scanf(" %d%*c",&j.idJeux);
	for(i = 0; i <= *nbJeux; i++)
	{
		if (tJeux[i].idJeux == j.idJeux)
		{
			printf("Trouve.\n");
			rep = 1;
			break;
		}
		rep = -1;
	}
	if (rep == -1)
	{
		printf("Ce jeu n'existe pas\n");
		return NULL;
	}
	rep = i;
	if (tJeux == NULL)
	{
		printf("Problème malloc\n");
		return NULL;
	}
	FILE *jeu_fichier;
	jeu_fichier = fopen("jeu.don","w");
	for(i = 0; i <= *nbJeux-1; i++)
	{
		if (i != rep)
			fprintf(jeu_fichier, "%d\t%s\t%s\t%d\n", tJeux[i].idJeux, tJeux[i].nom, tJeux[i].type, tJeux[i].quantite);
	}
	fclose(jeu_fichier);
	printf("Position : %d\nNombre de jeux : %d", rep, *nbJeux);
	if (rep >= *nbJeux+1)
		printf("Suppression impossible.");
	else
	{
		free(&tJeux[rep]);
		*nbJeux = *nbJeux - 1;
    	for (i = rep; i < *nbJeux; i++)
         	tJeux[i] = tJeux[i+1];
	}
	return tJeux;
}

void retourJeux(void)
{
	/*
		Nom:		retourJeux
		Finalité:	 Le retour déclenche l’examen des réservations pour peut être transformer une réservation en emprunt et faire parvenir le jeu à l’adhérent l’ayant réservé. 

		Description générale:
			Modifie la quantité d'un jeu, enlève un emprunt, et transforme (si possible) une réservation en emprunt.

		Variables:


	*/
	printf("Saisir l'ID de l'adhérent : ");
	//Saisie de l'ID de l'adhérent
	//Check quel jeu est emprunté par l'adhérent dans la liste emprunts.
	//Si aucun jeu n'est emprunté par l'adhérent -> Stop.
	//Si un jeu est emprunté par l'adhérent :
	//Suppresion de l'emprunt.
	//+1 dans la quantité du jeu en question dans tJeux.
	//Check si l'adhérent à d'autre réservation dans la liste réservations.
	//Si l'adhérent n'a aucune autre réservation -> Stop. 
	//Si l'adhérent a une autre réservation :
	//Prendre la réservation de l'adhérent la plus haute dans la liste réservation
	//Transformer la réservation en emprunt
	//-1 dans la quantité du jeu en question dans tJeux.
}




void menu(void)
{
	/*
		Nom:		menu
		Finalité:	Accéder aux fonctions nécessaires au bon fonctionnement de la gestion d'une ludothèque facilement.

		Description générale:
			Choisir une fonction en tapant un nombre.

		Variables:
			rep		Réponse de l'utilisateur
			bug		Retour d'une fonction si il y a
			get		Variable utilisé pour appuyer sur une touche pour continuer
	*/

	/* Note temporaire : Pour les noms des variables j'étais à court d'inspiration, je fixerais plus tard, ou alors si vous avez des idées vous pouvez fixe aussi */
	/* Note temporaire : Je vais trouver un autre moyen de rendre le menu un peu plus "pro" */
	int rep, bug;
	char get = '\n';
	Jeux *tJeux;
	int tMax = 100, nbJeux = 0;
	tJeux = chargeTJeux(tJeux, &nbJeux, &tMax);
	if(tJeux == NULL)
		return;
	while( get == '\n' )
	{
		system("@cls||clear"); //Clean de la console (fonctionne sur Windows/Linux/Mac)
		printf("Gestion d'une Ludothèque\n\n");
		printf("1. Jeux\n");
		printf("2. Adhérents\n");
		printf("3. Emprunts\n");
		printf("4. Réservation\n");
		scanf("%d%*c", &rep);
		switch(rep)
		{
			case 1:
				system("@cls||clear");
				printf("1. Afficher jeux disponibles\n2. Ajouter un jeu\n");
				printf("3. Retour d'un jeu\n4. Supprimer un jeu\n");
				printf("\n\n5. Retour au menu\n\n");
				scanf("%d%*c", &rep);
				switch(rep)
				{
					case 1:
						system("@cls||clear");
						afficheTJeux(tJeux, nbJeux);
						printf("\nAppuyez sur entrer pour continuer...\n");
						scanf("%c%*c", &get);
						break;
					case 2:
						system("@cls||clear");
						if(tJeux == NULL)
							return;
						tJeux = ajouterJeux(tJeux, &nbJeux, &tMax);
						printf("%d", &tJeux[1].idJeux);
						printf("\nAppuyez sur entrer pour continuer...\n");
						scanf("%c%*c", &get);
						break;
					case 3:
						system("@cls||clear");
						printf("C'est fonction n'est pas encore disponible.\n");
						printf("\nAppuyez sur entrer pour continuer...\n");
						scanf("%c%*c", &get);
						break;
					case 4:
						system("@cls||clear");
						if(tJeux == NULL)
							return;
						tJeux = supprimerJeux(tJeux, &nbJeux, &tMax);
						printf("\nAppuyez sur entrer pour continuer...\n");
						scanf("%c%*c", &get);
						break;
					case 5:
						break;
				}
				break;
			case 2:
				system("@cls||clear");
				// Fonction affichage emprunt
				printf("\nAppuyez sur entrer pour continuer...\n");
				scanf("%c%*c", &get);
				break;
			case 3:
				system("@cls||clear");
				// Fonction affichage emprunt
				printf("\nAppuyez sur entrer pour continuer...\n");
				scanf("%c%*c", &get);
				break;
			case 4:
				system("@cls||clear");
				// Fonction affichage emprunt
				printf("\nAppuyez sur entrer pour continuer...\n");
				scanf("%c%*c", &get);
				break;
			case 5:
				system("@cls||clear");
				// Fonction affichage emprunt
				printf("\nAppuyez sur entrer pour continuer...\n");
				scanf("%c%*c", &get);
				break;
			case 6:
				system("@cls||clear");
				// Fonction affichage emprunt
				printf("\nAppuyez sur entrer pour continuer...\n");
				scanf("%c%*c", &get);
				break;
			default:
				printf("\nErreur\n");
				break;
		}

	}	
}



Reservation* suppressionEnTete(Reservation *a) {
  Reservation *b;
  if (a==NULL) {
    printf("op interdite\n");
    exit(1);
  }
  b=a;
  a=a->suiv; 
  free(b);
  return a;
}

Reservation* supprimer(Reservation *a,int x) {
  if(a==NULL)
    return a;
  if(x < a -> idRes)
    return a;
  if(x==a->idRes)
    return suppressionEnTete(a);
  a->suiv=supprimer(a->suiv,x);
  return a;
}

Reservation* suppRes(Reservation *a) {
	int id;
	printf("Veuillez entrer le numéro de réservation à annuler :\n");
	scanf("%d", &id);
	a=supprimer(a,id);
	saveRes(a);
	return a;
}

void saveRes(Reservation *a) {
	FILE *flot;
	flot=fopen("Reservation.don","w");
	if (flot==NULL)
		printf("Problème d'ouverture du fichier des réservations en écriture, sauvegarde des réservations impossible\n");
	while (a!=NULL) {
		fprintf(flot, "%d\t%d\t%d\t%d/%d/%d\n", a->idRes,a->idAdherent,a->idJeu,a->dateR.jour,a->dateR.mois,a->dateR.annee);
		a=a->suiv;
	}
	fclose(flot);

}




Booleen vide(Emprunt *e) {
    if(e==NULL)
    	return vrai;
	return faux;
}

void afficherListeEmprunts(Emprunt *e, Jeux tJeux[],int taille_logique, int taille_logique_A, Adherent* tAdherent[]) {
	int adherent,jeu;
	int i;
	printf("\nAffichage de la liste des emprunts en cours :\n");
	printf("\nDate d'emprunt :\tId de l'emprunt\t\tNom et prénom de l'adhérent :\t\tNom du jeu :\n");
	while (!vide(e)){
		printf("%d/%d/%d\t\t\t%d\t\t\t",e->dateEmprunt.jour,e->dateEmprunt.mois,e->dateEmprunt.annee,e->idEmprunt);
		adherent=trouveNumAdherent(e->idAdherent,tAdherent,taille_logique_A);
		if (adherent==-1)
			printf("Inconnu\n");
		else
			printf("%s\t%s\t", tAdherent[adherent]->nom,tAdherent[adherent]->prenom);
		jeu=trouveNumJeu(e->idJeu,tJeux,taille_logique);
		if (jeu==-1)
			printf("Inconnu\n");
		else
			printf("\t%s\n",tJeux[jeu].nom);
		e=e->suiv;
	}
	printf("\n");
}

int trouveNumJeu(int id, Jeux tJeu[], int taille_logique)
{
	int inf=0, sup=taille_logique-1, milieu;
	while (inf <= sup) {
		milieu = (inf + sup)/2;
		if (id == tJeu[milieu].idJeux)
			return milieu;
		if (id < tJeu[milieu].idJeux)
			inf = milieu+1;
		else
			sup = milieu+1;
	}
	return -1;
}

int trouveNumAdherent(int id, Adherent* tAdherent[], int taille_logique_A)
{
	int inf=0, sup=taille_logique_A-1, milieu;
	while (inf <= sup) {
		milieu = (inf + sup)/2;
		if (id == tAdherent[milieu]->idAdherent)
			return milieu;
		if (id < tAdherent[milieu]->idAdherent)
			inf = milieu+1;
		else
			sup = milieu+1;
	}
	return -1;
}

Emprunt* chargeListeEmprunts(void) {
	FILE *flot;
	flot=fopen("Emprunts.don","r");
	Emprunt *e;
	int emprunt,adherent,jeu;
	Date date;
	printf("\nChargement du fichier des emprunts en mémoire...\n");
	if (flot==NULL) {
		printf("Une erreur est survenur lors de l'ouverture du fichier des emprunts ! (fichier introuvable)\n");
		exit(-1);
	}
	e=listenouv();
	date=lireFichier(flot,&emprunt,&adherent,&jeu);
	e=inserer(e,emprunt,adherent,jeu,date);
	while(!feof(flot)) {
		date=lireFichier(flot,&emprunt,&adherent,&jeu);
		e=inserer(e,emprunt,adherent,jeu,date);		
	}
	printf("Chargement du fichier des emprunts en mémoire réussis !\n");
	fclose(flot);
	return e;
}

Date lireFichier(FILE *flot, int *emprunt, int *adherent, int *jeu) {
	Date d;
	fscanf(flot,"%d%d%d%d/%d/%d",emprunt,adherent,jeu,&d.jour,&d.mois,&d.annee);
	return d;
}

Emprunt* listenouv(void) {
    Emprunt *e;
    e=NULL;
    return e;
}

Emprunt* insertionEnTete(Emprunt *s, int emprunt, int adherent, int jeu, Date date) {
	Emprunt *e;
	e=(Emprunt *)malloc(sizeof(Emprunt));
	if (e==NULL) {
		printf("Une erreur est survenue lors de la création de la liste ! (malloc)\n");
		exit(-1);
	}
	e->idEmprunt=emprunt;
	e->idAdherent=adherent;
	e->idJeu=jeu;
	e->dateEmprunt=date;
	e->suiv=s;
	return e;
}

Emprunt* inserer(Emprunt *e, int emprunt, int adherent, int jeu, Date date) {
	if (e==NULL)
		return insertionEnTete(e,emprunt,adherent,jeu,date);
	if (emprunt<e->idEmprunt)
		return insertionEnTete(e,emprunt,adherent,jeu,date);
	if (emprunt==e->idEmprunt)
		return e;
	e->suiv=inserer(e->suiv,emprunt,adherent,jeu,date);
	return e;
}


Booleen videR(Reservation *r) {
    if(r==NULL)
    	return vrai;
	return faux;
}

void afficherListeResa(Reservation *r, Adherent* tAdherent[], int taille_logique_A) {
	int id, adherent;
	printf("\nAffichageVeuillez entrer l'identifiant du jeu pour lequel vous souhaitez regarder la liste des réservations :\n");
	scanf("%d",&id);
	printf("\nAffichage de la liste des réservation en cours pour ce jeu :\n");
	printf("\nDate de réservation : \tId de la réservation :\t Nom et prénom de l'adhérent :\n");
	while (!videR(r)){
		if (r->idJeu==id) {
			printf("\t%d/%d/%d\t\t\t%d\t\t\t\n",r->dateR.jour,r->dateR.mois,r->dateR.annee,r->idRes);
			adherent=trouveNumAdherent(r->idAdherent,tAdherent,taille_logique_A);
			if (adherent==-1)
				printf("Inconnu\n");
			else
				printf("%s\t%s\t", tAdherent[adherent]->nom,tAdherent[adherent]->prenom);
		}
		r=r->suiv;
	}
	printf("\n");
}

Reservation* chargeListeResa(void) {
	FILE *flot;
	flot=fopen("Reservation.don","r");
	Reservation *e;
	int resa,adherent,jeu;
	Date date;
	printf("Chargement du fichier des réservations en mémoire...\n");
	if (flot==NULL) {
		printf("Une erreur est survenur lors de l'ouverture du fichier des réservations ! (fichier introuvable)\n");
		exit(-1);
	}
	e=listenouvR();
	date=lireFichierR(flot,&resa,&adherent,&jeu);
	e=insererR(e,resa,adherent,jeu,date);
	while(!feof(flot)) {
		date=lireFichierR(flot,&resa,&adherent,&jeu);
		e=insererR(e,resa,adherent,jeu,date);		
	}
	printf("Chargement du fichier des réservations en mémoire réussis !\n");
	fclose(flot);
	return e;
}

Date lireFichierR(FILE *flot, int *resa, int *adherent, int *jeu) {
	Date d;
	fscanf(flot,"%d%d%d%d/%d/%d",resa,adherent,jeu,&d.jour,&d.mois,&d.annee);
	return d;
}

Reservation* listenouvR(void) {
    Reservation *r;
    r=NULL;
    return r;
}

Reservation* insertionEnTeteR(Reservation *s, int resa, int adherent, int jeu, Date date) {
	Reservation *r;
	r=(Reservation *)malloc(sizeof(Reservation));
	if (r==NULL) {
		printf("Une erreur est survenue lors de la création de la liste ! (malloc)\n");
		exit(-1);
	}
	r->idRes=resa;
	r->idAdherent=adherent;
	r->idJeu=jeu;
	r->dateR=date;
	r->suiv=s;
	return r;
}

Reservation* insererR(Reservation *r, int resa, int adherent, int jeu, Date date) {
	if (r==NULL)
		return insertionEnTeteR(r,resa,adherent,jeu,date);
	if (resa<r->idRes)
		return insertionEnTeteR(r,resa,adherent,jeu,date);
	if (resa==r->idRes)
		return r;
	r->suiv=insererR(r->suiv,resa,adherent,jeu,date);
	return r;
}
