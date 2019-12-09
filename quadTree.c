#include <stdio.h>
#include <stdlib.h>
#include <MLV/MLV_all.h>
#include <unistd.h>
#include <time.h>
#define TAILLE 512
#define TAILLE_X 1500
#define TAILLE_Y 700
#define ALPHA 255 //c'est le coefficient de transparence, qu'on fixe pour rendre plus simple certaines fonctions

typedef struct noeud{
	int color;
	struct noeud *f1;
	struct noeud *f2;
	struct noeud *f3;
	struct noeud *f4;
}Noeud,*Arbre;
//-----------------------------------------------------
//-----------------------------------------------------
Arbre Alloue(int color){
	Arbre tmp=(Arbre)malloc(sizeof(Noeud));
	if (tmp==NULL) return NULL;
	tmp->color=color;
	tmp->f1=NULL;
	tmp->f2=NULL;
	tmp->f3=NULL;
	tmp->f4=NULL;
	return tmp;
}
//-----------------------------------------------------
//-----------------------------------------------------
int comparearbre(Arbre a,Arbre b){
	//si a est vide et b ne l'est pas, ou inversement, on renvoie 0
	//si les 2 sont vides, on renvoie 1
	//si a->color!=b->color, on renvoie 0
	//sinon on compare les fils des 2 arbres
	if (a==NULL && b==NULL) return 1;
	if (a==NULL) return 0; //ici on sait donc que b n'est pas vide donc on peut renvoyer 0
	if (b==NULL) return 0;
	if (a->color!=b->color)
		return 0;
	return (comparearbre(a->f1,b->f1) && comparearbre(a->f2,b->f2) && comparearbre(a->f3,b->f3) && comparearbre(a->f4,b->f4));
}
//-----------------------------------------------------
//-----------------------------------------------------
void intervertit(Arbre *P1index,Arbre *P2index,int numP1,int numP2, int h1, int h2){
	//h1 correspond à la hauteur qu'on veut atteindre dans *P1index, et de même pour h2 avec *P2index
	if (h1==0 && h2==0){
		Arbre tmp=NULL;
		tmp=*P1index;
		*P1index=*P2index;
		*P2index=tmp;
	}
	//on va regarder quels fils de *P1index et de *P2index on doit sélectionner
	else if (h1>0){
		switch(numP1%10){
			case 1: intervertit(&((*P1index)->f1),P2index,numP1/10,numP2,h1-1,h2);
					break;
			case 2: intervertit(&((*P1index)->f2),P2index,numP1/10,numP2,h1-1,h2);
					break;
			case 3: intervertit(&((*P1index)->f3),P2index,numP1/10,numP2,h1-1,h2);
					break;
			case 4: intervertit(&((*P1index)->f4),P2index,numP1/10,numP2,h1-1,h2);
					break;
		}
	}
	else{
		switch(numP2%10){
			case 1 :intervertit(P1index,&((*P2index)->f1),numP1,numP2/10,h1,h2-1);
					break;
			case 2 :intervertit(P1index,&((*P2index)->f2),numP1,numP2/10,h1,h2-1);
					break;
			case 3 :intervertit(P1index,&((*P2index)->f3),numP1,numP2/10,h1,h2-1);
					break;
			case 4 :intervertit(P1index,&((*P2index)->f4),numP1,numP2/10,h1,h2-1);
					break;
		}
	}
}
//-----------------------------------------------------
//-----------------------------------------------------
void swap(Arbre *a, int numP1, int numP2,int h){
	intervertit(a,a,numP1,numP2,h,h);
}
//-----------------------------------------------------
//-----------------------------------------------------
void echangealea16P(Arbre *a){
	//on va faire 100 échanges de 2 pièces, pour que *a ait très peu de chance d'être identique à l'arbre *a au départ
	int c,b,i=0,P1,P2,div_int_c,div_int_b;
	for(;i<100;i+=1){
		c=rand()%16;
		b=rand()%16;
		if (c==b)
			b=(b+1)%16;
		//a et b vont de 0 à 15, or pour la fonction echange2piece, les nombres sont notés différamment:
		//chacun des chiffres représente le fils de *a utilisé (par exemple : 13 représente (*a)->f3->f1)
		//on doit donc convertir a et b
		div_int_c=c/4;
		div_int_b=b/4;
		P1=10*(div_int_c+1)+((c%4)+1);
		P2=10*(div_int_b+1)+((b%4)+1);
		swap(a,P1,P2,2);
	}
}
//-----------------------------------------------------
//-----------------------------------------------------
void echangealea64P(Arbre *a){
	srand(time(NULL));
	//on va faire 1000 échanges de 2 pièces, pour que *a ait très peu de chance d'être identique à l'arbre *a au départ
	int c,b,i=0,P1,P2,div1c,div2c,div1b,div2b;
	for(;i<1000;i+=1){
		c=rand()%64;
		b=rand()%64;
		if (c==b)
			b=(b+1)%64;
		//a et b vont de 0 à 63, or pour la fonction echange2piece, les nombres sont notés différamment:
		//chacun des chiffres représente le fils de *a utilisé (par exemple : 312 représente (*a)->f2->f1->f3)
		//on doit donc convertir a et b
		div1c=c/4;
		div2c=c/16;
		div1b=b/4;
		div2b=b/16;
		P1=100*(div2c+1)+10*((div1c%4)+1)+((c%4)+1);
		P2=100*(div2b+1)+10*((div1b%4)+1)+((b%4)+1);
		swap(a,P1,P2,3);
	}
}
//-----------------------------------------------------
//-----------------------------------------------------
MLV_Image *set_to_white(){
	/**
 	 * \brief	initialise une image en blanc.
 	 * \return	une image blanc de 255X255. 	
	*/
	MLV_Image *image=MLV_create_image(TAILLE,TAILLE);
	int i,j;
	for(i=0; i<TAILLE; i++)
		for(j=0; j<TAILLE; j++)
			MLV_set_pixel_on_image(i,j,MLV_COLOR_WHITE,image);
	return (image);
}
//-----------------------------------------------------
//-----------------------------------------------------
int same_color(MLV_Image *image,int x,int y,int taille){
	/**
 	 * \brief	trouve si un carré est d'une même couleur.
 	 * \param	image une image a découper en carré.
 	 * \param	x la coordonnée x du coin haut gauche du carré.
 	 * \param	y la coordonnée y du coin haut gauche du carré.
 	 * \param	taille du côté du carré.
 	 * \return	-1 si le carré n'est pas d'une couleur uniforme.
 	 sinon renvoie la couleur du premier pixel.  	
	*/
	int color,temp,i,j;
	MLV_get_pixel_on_image(image,x,y,&color,NULL,NULL,NULL);
	for(i=0; i<taille; i++){
		for(j=0; j<taille; j++){
			MLV_get_pixel_on_image(image,x+i,y+j,&temp,NULL,NULL,NULL);
			if(temp!=color)
				return (-1);
		}
	}
	return color;
}
//-----------------------------------------------------
//-----------------------------------------------------
Arbre image_to_tree_aux(MLV_Image *image,int x,int y,int taille){
	/**
 	 * \brief	fonction recursive qui convertit un type MLV_Image en 
 	 Arbre quadratique.
 	 * \param	image une image à convertir.
 	 * \param	x la coordonnée x du coin haut gauche du carré.
 	 * \param	y la coordonnée y du coin haut gauche du carré.
 	 * \param	taille du côté du carré.
 	 * \return	l'arbre quadratique assosié à l'image. 	
	*/
	Arbre a=Alloue(-1);
	
	int color=same_color(image,x,y,taille);

	if(color!=-1 && taille<TAILLE/8){
		a->color=color;
		return a;
	}
	a->f1=image_to_tree_aux(image, x, y, taille/2);
	a->f2=image_to_tree_aux(image, x+taille/2, y, taille/2);
	a->f3=image_to_tree_aux(image, x+taille/2, y+taille/2, taille/2);
	a->f4=image_to_tree_aux(image, x, y+taille/2, taille/2);
	return a;
}
//-----------------------------------------------------
//-----------------------------------------------------
Arbre image_to_tree(MLV_Image *image){
	/**
 	 * \brief	convertit un type MLV_Image en Arbre quadratique.
  	 * \return	l'arbre quadratique assosié a l'image. 	
	*/
	return (image_to_tree_aux(image,0,0,TAILLE));
}
//-----------------------------------------------------
//-----------------------------------------------------
void tree_to_image_aux(Arbre a, MLV_Image *image, int x, int y, int taille){
	/**
 	 * \brief	fonction recursive qui convertit un Arbre quadratique en MLV_Image.
 	 * \param	a : l'arbre a mettre en image.
 	 * \param	image l'image associée a l'arbre.
 	 * \param	x la coordonnée x du coin haut gauche du carré.
 	 * \param	y la coordonnée y du coin haut gauche du carré.
 	 * \param	taille du côté du carré.
 	*/

	if(a==NULL)
		return;
	
	int val=a->color,i,j;
	if(val!=-1){
		MLV_Color color=MLV_convert_rgba_to_color(val,val,val,ALPHA);
		for(i=0; i<taille; i++){
			for(j=0; j<taille; j++){
				MLV_set_pixel_on_image(x+i,y+j,color,image);
			}
		}		
	}
	tree_to_image_aux(a->f1, image, x, y, taille/2);
	tree_to_image_aux(a->f2, image, x+(taille/2), y, taille/2);
	tree_to_image_aux(a->f3, image, x+(taille/2), y+(taille/2), taille/2);
	tree_to_image_aux(a->f4, image, x, y+(taille/2), taille/2);
}
//-----------------------------------------------------
//-----------------------------------------------------
MLV_Image *tree_to_image(Arbre a){
	/**
 	 * \brief	convertit un Arbre quadratique en MLV_Image.
 	 * \param	a : l'arbre à mettre en image.
 	 * \return	l'image associée a l'arbre.. 	
 	*/
	MLV_Image *image=MLV_create_image(TAILLE,TAILLE);
	tree_to_image_aux(a,image,0,0,TAILLE);
	return image;
}
//-----------------------------------------------------
//-----------------------------------------------------
int exist(char fichier[]){
	/**
 	 * \brief	vérifie que un fichier existe et est de type .jpg.
 	 * \param	fichier : le chemin vers un fichier.
 	 * \return	1 si le fichier n'existe pas ou ne se finit pas par .jpg.
 	 sinon 0; 
 	 */

	//le fichier finit par .jpg
	int i;
	for(i=0; fichier[i]!='.'; i++){
		if(fichier[i]=='\0')
			return 1;
	}
	if(fichier[i+1]!='j' || fichier[i+2]!='p' || fichier[i+3]!='g')
		return 1;

	//le fichier existe bien
	FILE *file;
	if((file=fopen(fichier,"r"))){
		fclose(file);
		return 0;
	}
	return 1;
}
//-----------------------------------------------------
//-----------------------------------------------------
MLV_Image *load_image_aux(char *fichier){
	/**
 	 * \brief	charge une image à partir du nom du fichier.
 	 * \param	fichier : la chaine de caractère correspondant 
 	 à un fichier.
 	 * \return	l'image assosiée au nom du fichier. 
 	 */
	int h,w;
	MLV_Image *image;
	image=MLV_load_image(fichier);
	MLV_get_image_size(image,&w,&h);
	
	if(h==w){
		MLV_resize_image_with_proportions(image,TAILLE,TAILLE);
		return (image);
	}
	if(h>w){
		MLV_resize_image_with_proportions(image,TAILLE*(w+0.0)/h,TAILLE);
		w=TAILLE*(w+0.0)/h;
		h=TAILLE;
	}
	if(w>h){
		MLV_resize_image_with_proportions(image,TAILLE,TAILLE*(h+0.0)/w);
		h=TAILLE*(h+0.0)/w;
		w=TAILLE;
	}
	MLV_Image *image_return=set_to_white();
	MLV_draw_image_on_image(image, image_return,(TAILLE-w)/2,(TAILLE-h)/2);
	return (image_return);
}
//-----------------------------------------------------
//-----------------------------------------------------
MLV_Image *load_image(){
	/**
 	 * \brief	charge une image.
 	 * \return 	l'image associée à un nom que l'utilisateur a rentré
 	 dans input box. 
 	 */
	char *fichier;
	int n=1;
	while(n || exist(fichier)){
		MLV_wait_input_box((TAILLE_X-225)/2,500,225,40,MLV_COLOR_WHITE,MLV_COLOR_BLUE,MLV_COLOR_WHITE,
			"nom du fichier:",&fichier);
		n=0;
	}
	return load_image_aux(fichier);
}
//-----------------------------------------------------
//-----------------------------------------------------
void convert_to_grey(MLV_Image *image){
	/**
 	 * \brief	convertit une image en 255 nuance de gris.
 	 * \param	image : une image quelconque à convertir en gris.
 	*/
	int red,green,blue,alpha;
	int temp=0,i,j;
	MLV_Color color;
	for(i=0; i<TAILLE; i++){
		for(j=0; j<TAILLE; j++){
			MLV_get_pixel_on_image(image,i,j,&red,&green,&blue,&alpha);	
			temp=red*0.299 +green*0.587 +blue*0.114;
			//les valeurs des constantes viennent de la librairie open CV
			color=MLV_convert_rgba_to_color(temp,temp,temp,ALPHA);
			MLV_set_pixel_on_image( i, j, color, image);
		}
	}
}
//-----------------------------------------------------
//-----------------------------------------------------
void draw_menu(){
	/**
 	 * \brief	affiche tous les boutons du menu.
	*/
	
	//charger image
	MLV_draw_adapted_text_box((TAILLE_X-122)/2,100,"charger image",10,
	MLV_COLOR_WHITE, MLV_COLOR_BLUE, MLV_COLOR_WHITE, MLV_TEXT_CENTER);
	
	//charger quad_tree
	MLV_draw_adapted_text_box((TAILLE_X-148)/2,150,"charger quad_tree",10,
	MLV_COLOR_WHITE, MLV_COLOR_BLUE, MLV_COLOR_WHITE, MLV_TEXT_CENTER);
	
	//puzzle 16 pieces
	MLV_draw_adapted_text_box((TAILLE_X-133)/2,200,"puzzle 16 pieces",10,
	MLV_COLOR_WHITE, MLV_COLOR_BLUE, MLV_COLOR_WHITE, MLV_TEXT_CENTER);
	
	//puzzle 64 pieces
	MLV_draw_adapted_text_box((TAILLE_X-133)/2,250,"puzzle 64 pieces",10,
	MLV_COLOR_WHITE, MLV_COLOR_BLUE, MLV_COLOR_WHITE, MLV_TEXT_CENTER);
	
	//quitter
	MLV_draw_adapted_text_box((TAILLE_X-63)/2,300,"quitter",10,
	MLV_COLOR_WHITE, MLV_COLOR_BLUE, MLV_COLOR_WHITE, MLV_TEXT_CENTER);	
}
//-----------------------------------------------------
//-----------------------------------------------------
void draw_game(MLV_Image *image_ref,MLV_Image *image_puz){
	/**
 	 * \brief	affiche tous les elements pour jouer.
 	 * \param	image_ref : l'image à reconstituer.
 	 * \param	image_puz : l'arbre quadratique qu'il faut résoudre.
	*/
	
	MLV_clear_window(MLV_COLOR_BLACK);	
	//image de ref	
	MLV_draw_image(image_ref,(TAILLE_X/2-TAILLE)/2,(TAILLE_Y-TAILLE)/2);
	//qtree de puzz
	MLV_draw_image(image_puz,(3*TAILLE_X/2-TAILLE)/2,(TAILLE_Y-TAILLE)/2);
	//bouton
		//menu
		draw_menu();
		//aide
		MLV_draw_adapted_text_box((TAILLE_X-50)/2,500,"aide",10,
		MLV_COLOR_WHITE, MLV_COLOR_BLUE, MLV_COLOR_WHITE, MLV_TEXT_CENTER);	

	MLV_actualise_window();
}
//-----------------------------------------------------
//-----------------------------------------------------
int in_menu(int x,int y){
	/**
 	 * \brief	touve si le clic est dans l'un des bouton
 	 du menu.
 	 * \param	x	la coordonée en x du clic.
 	 * \param	y	la coordonée en y du clic.
 	 * \return	0 si le clic n'est dans aucun des bouton du menu.
 	 1 si le clic est sur charger image.
 	 2 si le clic est sur charger qtree.
 	 3 si le clic est sur puzzle 16 pieces.
 	 4 si le clic est sur puzzle 64 pieces.
 	 5 si le clic est sur quitter.
 	 */
	//charger image
	if(x>=(TAILLE_X-122)/2 && x<=(TAILLE_X+122)/2 && y>=100 && y<=135)
		return 1;

	//charger quad_tree
	if(x>=(TAILLE_X-148)/2 && x<=(TAILLE_X+148)/2 && y>=150 && y<=185)
		return 2;

	//puzzle 16 pieces
	if(x>=(TAILLE_X-133)/2 && x<=(TAILLE_X+133)/2 && y>=200 && y<=235)
		return 3;

	//puzzle 64 pieces
	if(x>=(TAILLE_X-133)/2 && x<=(TAILLE_X+133)/2 && y>=250 && y<=285)
		return 4;

	//quitter
	if(x>=(TAILLE_X-63)/2  && x<=(TAILLE_X+63)/2  && y>=300 && y<=335)
		return 5;	

	return 0;
}
//-----------------------------------------------------
//-----------------------------------------------------
int in_aide(int x,int y){
	/**
 	 * \brief	trouve si le clic est sur le bouton aide.
 	 * \param	x : la coordonnée en x du clic.
 	 * \param	y : la coordonnée en y du clic.
 	 * \return	0 si le clic n'est pas sur le bouton aide.
 	 sinon 1.
 	 */

	if(!(x>=(TAILLE_X-50)/2 && x<=(TAILLE_X+50)/2 && y>=500 && y<=535))
		return 0;
	return 1;
}
//-----------------------------------------------------
//-----------------------------------------------------
int in_image_aux(int x,int x1,int x2,int y,int y1,int y2){
	/**
 	 * \brief	trouve sur quelle pièce du puzzle on clique.
 	 * \param	x	la coordonnée en x du clic.
 	 * \param	x1	la droite droite du carré.
 	 * \param	x2	la droite gauche du carré.
 	 * \param	y	la coordonnée en y du clic.
 	 * \param	y1	la droite haute du carré.
 	 * \param	y1	la droite bas du carré.
 	 * \return	un entier avec comme unité la valeur du
 	 premier noeud, dizaine le deuxième noeud, centaine 
 	 le troisième(seulement utilisé pour un puzzle à 64 pièces). 	
	---------------------
	| 11 | 21 | 12 | 22 |
	|-------------------|
	| 41 | 31 | 42 | 32 |
	|-------------------| exemple de numerotation pour 16 pieces
	| 14 | 24 | 13 | 23 |
	|-------------------|
	| 44 | 34 | 43 | 33 |
	|-------------------|
	*/
	if((x2-x1<=TAILLE/8) || (y2-y1<=TAILLE/8))
		return 0;

	if(x>=x1 && x<=(x2+x1)/2 && y>=y1 && y<=(y2+y1)/2)
		return (1+10*in_image_aux(x, x1, (x2+x1)/2, y, y1, (y2+y1)/2));
	if(x>=(x2+x1)/2 && x<=x2 && y>=y1 && y<=(y2+y1)/2)
		return (2+10*in_image_aux(x, (x2+x1)/2, x2, y, y1, (y2+y1)/2));
	if(x>=(x2+x1)/2 && x<=x2 && y>=(y2+y1)/2 && y<=y2)
		return (3+10*in_image_aux(x, (x2+x1)/2, x2, y, (y2+y1)/2, y2));
	if(x>=x1 && x<=(x2+x1)/2 && y>=(y2+y1)/2 && y<=y2)
		return (4+10*in_image_aux(x, x1, (x2+x1)/2, y, (y2+y1)/2, y2));
	return 0;
}
//-----------------------------------------------------
//-----------------------------------------------------
int in_image(int x,int y){
	/**
 	 * \brief	trouve si le clic est sur le puzzle.
 	 * \param	x : la coordonnée en x du clic.
 	 * \param	y : la coordonnée en y du clic.
 	 * \return	0 si le clic n'est pas sur l'image.
 	 sinon un entier de 111 a 444, determiné par la fonction
 	 auxiliaire.
 	 */
	if(!(x>=(3*TAILLE_X/2-TAILLE)/2 && x<=(3*TAILLE_X/2+TAILLE)/2 && y>=(TAILLE_Y-TAILLE)/2 && y<=(TAILLE_Y+TAILLE)/2))
		return 0;
	return in_image_aux(x,(3*TAILLE_X/2-TAILLE)/2,(3*TAILLE_X/2+TAILLE)/2,y,(TAILLE_Y-TAILLE)/2,(TAILLE_Y+TAILLE)/2);
}
//-----------------------------------------------------
//-----------------------------------------------------
void aideaux(MLV_Image *image, Arbre base, Arbre puzzle, int x, int y, int taille,int h){
	//on va comparer les noeuds (qui sont des arbres) de hauteur h de base et de puzzle.
	//S'ils ne sont pas égaux, on fait apparaître et disparaître 5 fois un rectangle jaune sur l'emplacement de la pièce de l'arbre puzzle
	if (h!=0){
		aideaux(image,base->f1,puzzle->f1,x,y,taille/2,h-1);
		aideaux(image,base->f2,puzzle->f2,x+taille/2,y,taille/2,h-1);
		aideaux(image,base->f3,puzzle->f3,x+taille/2,y+taille/2,taille/2,h-1);
		aideaux(image,base->f4,puzzle->f4,x,y+taille/2,taille/2,h-1);
	}
	else{
		int i=0;
		if (comparearbre(base,puzzle)==0) {
			for (;i<5;i++){
				MLV_draw_filled_rectangle(x,y,taille,taille,MLV_COLOR_YELLOW);
				
			}
		}
	}
}
//-----------------------------------------------------
//-----------------------------------------------------
void fonctionaide(MLV_Image *image ,Arbre base,Arbre a,int hauteur){
	int i;
	for(i=0;i<6;i++){
		draw_game(image,tree_to_image(a));
		MLV_wait_milliseconds(70);

		aideaux(image,base,a,(3*TAILLE_X/2-TAILLE)/2,(TAILLE_Y-TAILLE)/2,TAILLE,hauteur);
		MLV_actualise_window();
		MLV_wait_milliseconds(70);

	}
	draw_game(image,tree_to_image(a));
}
//-----------------------------------------------------
//-----------------------------------------------------
void jouer(){
	MLV_Image *image=set_to_white();
	Arbre qtree=NULL;
    int x,y,h=-1,i;
	int zone,tmp;
	draw_menu();
	MLV_actualise_window();
	for(i=0; i<500; i++){
		MLV_wait_mouse(&x,&y);
		//sur l'image 
		if(0!=(zone=in_image(x,y))){
			if(h!=-1){
				tmp=zone;
				MLV_wait_mouse(&x,&y);
				if(0!=(zone=in_image(x,y))){
					if(h==2){
						swap(&qtree,tmp%100,zone%100,h);
					}
					if(h==3){
						swap(&qtree,tmp,zone,h);
					}
					draw_game(image,tree_to_image(qtree));
				}
			}
		}

		//dans le menu
		if(0!=(zone=in_menu(x,y))){
			switch(zone){
				case 1: //charge image
						image=load_image();
						convert_to_grey(image);
						qtree=image_to_tree(image);
						draw_game(image,tree_to_image(qtree));
						h=0;
						break;	

				//case 2://charger qtree
				case 3://16 piece
						if(h!=-1 && h!=3){//on ne peux pas re decouper le puzzle en 16 si il est couer en 64
							h=2;
							echangealea16P(&qtree);
							draw_game(image,tree_to_image(qtree));
						}
						break;
				case 4://64 piece
						if(h!=-1){
							h=3;
							echangealea64P(&qtree);
							draw_game(image,tree_to_image(qtree));
						}
						break;
                case 5: exit(1);
			}
		}

		//dans l'aide
		if(0!=(zone=in_aide(x,y))){
			fonctionaide(image,image_to_tree(image),qtree,h);
			
		}
		//il y a un problème lorsqu'on appuie sur le bouton aide sans mélanger: le jeu se termine anormalement
		if (i!=0 && comparearbre(image_to_tree(image),qtree)==1){
			MLV_clear_window(MLV_COLOR_BLACK);
			MLV_draw_text(745,350,"Vous avez gagné",MLV_COLOR_WHITE);
			MLV_actualise_window();
			MLV_wait_seconds(4);
			break;
		}
	}
}
//-----------------------------------------------------
//-----------------------------------------------------
int main(){
	srandom(time(NULL));
	MLV_create_window( "test puzzle", "image", TAILLE_X, TAILLE_Y);
    //-----------------------------------------------------
	//-----------------------------------------------------
	jouer();	

	return 0;
}
