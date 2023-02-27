/*
Angélique Delevingne
M1 Informatique 
19000955

Réseau de neurones pour OR et AND


Ce programme entraine deux réseaux de neurones OR et AND en simultannée avec deux thtread différents.
Le réseau de neurones est composé :
    nombre de neurones de couche d'entrée : 2
    nombre de neurones de couche cachée : 4
    nombre de neurones de couche de sortie : 1

Le réseau de neurones est entrainé sur toutes les combinaisons du OR et AND.
Il itère sur ces jeux de données 30000 fois.
A la fin de l'entrainement je teste pour chaque entrée mon réseau de neurones
et j'affiche le résultat obtenu et le résultat attendu

Exemple : 

In main: creating thread 0
In main: creating thread 1
Le thread #0, exécute le traitement AND 
Le thread #1, exécute le traitement OR 
AND Entrée : 0.000000 0.000000 - Sortie obtenue -> -0.000209  - Sortie attendue -> 0.000000 
AND Entrée : 0.000000 1.000000 - Sortie obtenue -> 0.000004  - Sortie attendue -> 0.000000 
AND Entrée : 1.000000 0.000000 - Sortie obtenue -> 0.000072  - Sortie attendue -> 0.000000 
AND Entrée : 1.000000 1.000000 - Sortie obtenue -> 0.996327  - Sortie attendue -> 1.000000 
FIN DU  thread #0, AND 
OR Entrée : 0.000000 0.000000 - Sortie obtenue -> 0.000012  - Sortie attendue -> 0.000000 
OR Entrée : 0.000000 1.000000 - Sortie obtenue -> 0.998198  - Sortie attendue -> 1.000000 
OR Entrée : 1.000000 0.000000 - Sortie obtenue -> 0.998173  - Sortie attendue -> 1.000000 
OR Entrée : 1.000000 1.000000 - Sortie obtenue -> 0.999837  - Sortie attendue -> 1.000000 
FIN DU  thread #1, OR 

Pour exécuter le programme : 
gcc rda_or_and.c  -lm -lpthread
./a.out

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define NUM_THREADS	2


// Nombre entrées couche 1
int C1_E=2;
// Nombre de sortie couche 1
int C1_S=4;
// Nombre entrées couche 2
int C2_E=4;
// Nombre de sortie couche 2
int C2_S=1;

typedef struct Matrice {
    int colonne;
    int ligne;
    long double  **  tab;
}Matrice;

typedef struct ReseauDeNeurone{
    Matrice * matrice_biais1;
    Matrice * matrice_biais2;

    Matrice * matrice_poids1;
    Matrice * matrice_poids2;

    Matrice * resultat;

}ReseauDeNeurone;

// Affiche le nom de la matrice et la matrice
void affiche_matrice(char * nom_matrice, Matrice *matrice ){
    printf("\nMatrice %s\n", nom_matrice);
    printf("=================================================\n");
    int i, o;
    for(o = 0; o<matrice->ligne; o++){
        for(i = 0; i<matrice->colonne; i++)
            printf(" %Lf ",matrice->tab[o][i] * 100000000000000000);
        printf("\n");
    }
    printf("=================================================\n");
}

// Création de la structure Matrice 
// Allocation de mémoire + ajout de valeurs
// Si valeurs vaut NULL alors retroune une Matrice aléatoire
// Sinon initialise la matrice avec les valeurs
Matrice * create_matrice(int ligne,int col, long double  * valeurs){
    
    Matrice * matrice_struct = malloc( sizeof (Matrice) );
   
    long double  **  matrice = malloc(ligne * sizeof (long double ) );
    for(int i = 0; i < ligne; i++){
       matrice[i] = malloc(col * sizeof *(matrice[i]));
    }
    int i, o,k;
    k=0;
    
    for(o = 0; o<ligne; o++)
    {
        for(i = 0; i<col; i++)
        {
            if( valeurs == NULL){
                long double x =(long double)rand()/(long double)(RAND_MAX);
                matrice[o][i] = x;
            }else{
                matrice[o][i] = valeurs[k];
            }
            k++;
        }    
    }
    
    matrice_struct->ligne=ligne;
    matrice_struct->colonne=col;
    matrice_struct->tab=matrice;
    
    return matrice_struct;
        
}
// Multiplication entre deux matrices
Matrice *  multiplication_matrices( Matrice * a, Matrice * b){
    int i,j,k,n,p,m;
    Matrice *  mul= create_matrice(a->ligne,b->colonne,NULL);
    n= a->ligne ;//nb ligne matrice 1
    m=b->ligne ;//nb ligne matrice 2
    p=b->colonne ;// nb de colonne de matrice 2
    for(i=0;i<n;i++)
    {
        for(j=0;j<p;j++)
        {
            mul->tab[i][j]=0;
            for(k=0;k<m;k++)
                {
                    mul->tab[i][j]+=a->tab[i][k]*b->tab[k][j];
                }
            }
        }
    return mul;
}
// Multiplication d'une matrice avec un nombre
Matrice * multiplication_matrice_double (Matrice * m1, long double l){
    int i,j;
    
    for(i = 0; i < m1->ligne; ++i)
    {
        for(j=0; j < m1->colonne; ++j)
        {
                m1->tab[i][j] = m1->tab[i][j] * l;
                
        }
        //printf("\n");
    }
    return m1;
}
// Addition de deux matrices
Matrice * addition_matrices (Matrice * m1, Matrice * b){
    int i,j;
    if ((m1->ligne != b->ligne )|| (m1->colonne != b->colonne)){
        printf("ERROR\n");
        exit(1);
    }
    
    for(i = 0; i < m1->ligne; ++i)
    {
        for(j=0; j < m1->colonne; ++j)
        {
                m1->tab[i][j] = m1->tab[i][j] + b->tab[i][j];

        }
       
    }
    return m1;
}
// Soustraction entre deux matrices
Matrice * soustraction_matrices (Matrice * m1, Matrice * b){
    int i,j;
    if ((m1->ligne != b->ligne )|| (m1->colonne != b->colonne)){
        printf("ERROR DE TAILLE\n");
        exit(1);
    }
    for(i = 0; i < m1->ligne; ++i)
    {
        for(j=0; j < m1->colonne; ++j)
        {
                m1->tab[i][j] = m1->tab[i][j] - b->tab[i][j];
            
        }
    }
    return m1;
}

// Création d'une matrice en prenant le "numero" de la ligne qu'on souhaite copier de la matrice "valeurs"
Matrice * copie_ligne_matrice(Matrice * valeurs,int numero){
    
    Matrice * matrice_struct = malloc( sizeof (Matrice) );
   
    long double  **  matrice = malloc(1 * sizeof (long double ) );
    
    matrice[0] = malloc(valeurs->colonne * sizeof *(matrice[0]));
    
    int i;
    
    for(i = 0; i<valeurs->colonne; i++)
    {
        matrice[0][i] = valeurs->tab[numero][i];
    }
            
    matrice_struct->ligne=1;
    matrice_struct->colonne=valeurs->colonne;
    matrice_struct->tab=matrice;

    return matrice_struct;
        
}

// Retourne une matrice transposée qui est le résultat de la réorganisation de la matrice d'origine "a" 
// en changeant lignes par colonnes et colonnes par lignes . 
Matrice * transpose_matrice(Matrice * a){

    Matrice * resultat=create_matrice(a->colonne,a->ligne,NULL);

     for(int i=0; i < a->ligne; i++){
        for(int j =0; j< a->colonne ;j++){
            resultat->tab[j][i]= a->tab[i][j];

        }
    }
    return resultat;

}
// Fonction  d'Activation 
Matrice * activation_tanh_matrices ( Matrice *  z, Matrice * a ){
    for (int ligne=0; ligne < z->ligne; ligne++){
        for (int colonne=0; colonne<z->colonne; colonne++){
            a->tab[ligne][colonne]=tanhl( z->tab[ligne][colonne]);
        }
    }
    
    return a;
}
// Fonction  dérivée d'Activation 
Matrice * activation_derivee_tanh_matrice(Matrice * a ){
    Matrice *res=create_matrice(a->ligne,a->colonne,NULL);
     for (int ligne=0; ligne < a->ligne; ligne++){
        for (int colonne=0; colonne< a->colonne; colonne++){ 
            res->tab[ligne][colonne]=1-(powl(tanhl( a->tab[ligne][colonne]),2));
        }
    }
    return res;

}
//Fonction de calcul de perte
long double  perte_matrices(Matrice * y_true, Matrice * y_pred){
    if ((y_true->ligne != y_pred->ligne )|| (y_true->colonne != y_pred->colonne)){
        printf("ERROR\n");
        exit(1);
    }
    Matrice * res=create_matrice(y_true->ligne,y_true->colonne,NULL);
    
    for(int i =0 ; i< y_true->ligne; i++){
        for(int j =0 ; j< y_true->colonne; j++){
            res->tab[i][j] = y_true->tab[i][j] -  y_pred->tab[i][j];
            
            res->tab[i][j] = powl(res->tab[i][j],2);
        }
    }

    long double  somme=0;
    for(int i =0 ; i< res->ligne; i++){
        for(int j =0 ; j< res->colonne; j++){
            somme += res->tab[i][j];
        }
    }
    
    long double s= somme /(res->ligne * res->colonne);
    return s;
}
//Dérivé de la fonction de calcul de perte
Matrice * perte_derivee_matrices( Matrice * y_true, Matrice * y_pred){
   
    if ((y_true->ligne != y_pred->ligne )|| (y_true->colonne != y_pred->colonne)){
        printf("PAS LA MEME TAILLE\n");
        exit(1);
    }
    Matrice * error=create_matrice(y_pred->ligne,y_pred->colonne,NULL);
    long double  nb_element= y_true->colonne * y_true->ligne;

    for(int i =0 ; i< y_true->ligne; i++){
        for(int j =0 ; j< y_true->colonne; j++){
            error->tab[i][j] = (2* ( y_pred->tab[i][j] - y_true->tab[i][j])) / nb_element;
        }
    }
    return error;
}

/*
La fonction back_propagation est une méthode pour entraîner un réseau de neurones, 
consistant à mettre à jour les poids "w" et les biais "b" de chaque neurone pour une couche
Elle vise à corriger les erreurs selon l'importance de la contribution de chaque élément à celles-ci
*/
Matrice * back_propagation(Matrice * z, Matrice *error,Matrice * w, Matrice * intput , Matrice *b,double lr){
    
    Matrice * z2_prime=activation_derivee_tanh_matrice(z);
    Matrice *  output_error= multiplication_matrices(z2_prime, error);
    
    Matrice *poids_transposee=transpose_matrice(w);
    Matrice *  intput_error=multiplication_matrices(output_error,poids_transposee);

    Matrice *intput_transpose=transpose_matrice(intput);
   
    Matrice *  weight_error=multiplication_matrices(intput_transpose, output_error);

    // MAJ des paramètre

    //MAJ POIDS
    Matrice * matrice_learning_rate= multiplication_matrice_double(weight_error, lr);
    soustraction_matrices(w, matrice_learning_rate);

    //MAJ BIAIS
    Matrice * matrice_learning_rate_biais= multiplication_matrice_double(output_error, lr);
    soustraction_matrices(b, matrice_learning_rate_biais);

    return intput_error;
}
/*
Fonction qui permet l'apprentissage et également l'utilisation 

Lorsque le paramètre apprentissage vaut 1 alors l'algorithme va faire "iteration_max" qui est de 30000 de 
l'étape propagation et back_propagation en corrigeant en fonction de l'erreur .
La fonction retourne une structure ReseauDeNeurone

Lorsque le paramètre apprentissage vaut 0 alors l'algorithme va faire une fois 
l'étape propagation avec le ReseauDeNeurone issus de l'apprentissage.
La fonction retourne une structure ReseauDeNeurone ayant le resultat obtenue

*/
ReseauDeNeurone * apprentissage_utilisation_reseau_neurone(long tid,int apprentissage, ReseauDeNeurone *r, Matrice * x_train ,  Matrice * y_train, int iteration_max, long double  learning_rate){
    int samples= x_train->ligne;
    // printf("samples :%d\n", samples);

    for(int i=0; i< iteration_max; i++){
        long double  err=0;

        for(int j =0; j< samples;j++){

            Matrice * matrice_x1=copie_ligne_matrice(x_train, j);
                //FORWARD
                //COUCHE 1
                Matrice * matrice_z1 = multiplication_matrices(matrice_x1,r->matrice_poids1 );
                
                addition_matrices(matrice_z1, r->matrice_biais1 );

                //ACTIVATION
                Matrice * matrice_x2=create_matrice(1,4,NULL);
                activation_tanh_matrices ( matrice_z1, matrice_x2 );
              
                //FORWARD
                //COUCHE 2
                Matrice * matrice_z2 = multiplication_matrices(matrice_x2,r->matrice_poids2 );
                addition_matrices(matrice_z2, r->matrice_biais2 );
                
                //ACTIVATION
                Matrice * matrice_activation2=create_matrice(1,1,NULL);
                activation_tanh_matrices ( matrice_z2, matrice_activation2 );
                r->resultat=matrice_activation2;
                
                
                if ( apprentissage == 1){
                    Matrice *y=copie_ligne_matrice(y_train,j);
                    

                    err += perte_matrices(y,matrice_activation2 );
                    long double a=perte_matrices(y,matrice_activation2 );
                   
                    Matrice * error = perte_derivee_matrices(y, matrice_activation2);
                    
                    Matrice * error1=back_propagation(matrice_z2, error,r->matrice_poids2 , matrice_x2 , r->matrice_biais2 ,learning_rate);
                   
                    Matrice * error2=back_propagation(matrice_z1, error1,r->matrice_poids1 , matrice_x1 , r->matrice_biais1 ,learning_rate);
                    
                }
                
            
                
        }
        if ( apprentissage == 1){
            err /= samples;
            // printf("THREAD %ld EPOCH %d %d error %Lf8 \n",tid, i+1, iteration_max, err);
        }
    }
    return r;

}
// Création du Reseau De Neurones
ReseauDeNeurone initialisation_reseau_neurone(){

    // initialisation DE MATRICE DE POIDS
    Matrice * matrice_poids1=create_matrice(C1_E,C1_S, NULL);
  
    // initialisation DE MATRICE DE BIAIS
    Matrice * matrice_biais1=create_matrice(1,C2_E, NULL);
   
    //  initialisation DE MATRICE DE POIDS 2
    Matrice * matrice_poids2=create_matrice(C2_E,1, NULL);
   
    // initialisation DE MATRICE DE BIAIS 2
    Matrice * matrice_biais2=create_matrice(C2_S,1, NULL);
  
    // initialisation DE Reseau De Neurone
    ReseauDeNeurone r1;
    r1.matrice_biais1=matrice_biais1;
    r1.matrice_biais2=matrice_biais2;
    r1.matrice_poids1=matrice_poids1;
    r1.matrice_poids2=matrice_poids2;

    return r1;

}

/*
Fonction qui va exécuter :
    pour le Thread 1 le OR 
        Il va faire l'apprentisage avec une table de vérité pour le OR
        Puis l'utilisation pour chaque entrée et renvoie le résultat obtenue du Réseau de neurones
    
    pour le Thread 1 le AND
        Il va faire l'apprentisage avec une table de vérité pour le AND
        Puis l'utilisation pour chaque entrée et renvoie le résultat obtenue du Réseau de neurones
*/
void *RDN(void *threadid){
    long double  x[8]= { 0, 0, 0,1,1,0,1,1};
    Matrice * x_train=create_matrice(4,2, x);
   
   long tid;
   tid = (long)threadid;
   if( tid == 1){
    printf("Le thread #%ld, exécute le traitement OR \n", tid);

    long double  y_or[4]= {0,1,1,1};
    Matrice * y_train_or=create_matrice(4,1, y_or);

    // RANDOM POIDS ET BIAIS
    ReseauDeNeurone r1=initialisation_reseau_neurone();

    // RDN OR
    // APPRENTISSAGE
    ReseauDeNeurone *r1_train=apprentissage_utilisation_reseau_neurone(tid,1,&r1, x_train,y_train_or,30000,0.1);
    //UTILISATION
    for (int i=0; i< 8; i=i+2){
        long double  x1[8]= { 0, 0, 0,1,1,0,1,1};
        long double  y_or[4]= {0,1,1,1};
        
        Matrice * x_test=create_matrice(1,2, &x1[i]);
        ReseauDeNeurone *obtenue = apprentissage_utilisation_reseau_neurone(tid,0,r1_train, x_test,y_train_or,1,0.1);
        printf("OR ");
        printf("Entrée : %Lf %Lf - Sortie obtenue -> %Lf  - Sortie attendue -> %Lf \n", x_test->tab[0][0] , x_test->tab[0][1], obtenue->resultat->tab[0][0], y_or[i/2]) ;
    
    }

    printf("FIN DU  thread #%ld, OR \n", tid);

   }else{
    printf("Le thread #%ld, exécute le traitement AND \n", tid);

    long double  y_and[4]= {0,0,0,1};
    Matrice * y_train_and=create_matrice(4,1, y_and);
    
    // RANDOM POIDS ET BIAIS
    ReseauDeNeurone r2=initialisation_reseau_neurone();

    // RDN AND
    // APPRENTISSAGE
    ReseauDeNeurone *r2_train= apprentissage_utilisation_reseau_neurone(tid,1,&r2, x_train,y_train_and,30000,0.1);
    //UTILISATION
    for (int i=0; i< 8; i=i+2){
        long double  x1[8]= { 0, 0, 0,1,1,0,1,1};
        Matrice * x2_test=create_matrice(1,2, &x1[i]);
        ReseauDeNeurone *obtenue2 = apprentissage_utilisation_reseau_neurone(tid,0,r2_train, x2_test,y_train_and,1,0.1);
        printf("AND");
        printf(" Entrée : %Lf %Lf - Sortie obtenue -> %Lf  - Sortie attendue -> %Lf \n", 
        x2_test->tab[0][0] , x2_test->tab[0][1], obtenue2->resultat->tab[0][0], y_and[i/2]) ;
    
    }

    printf("FIN DU  thread #%ld, AND \n", tid);

   }
   
   pthread_exit(NULL);
}
int main(){
    
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;
    for(t=0;t<NUM_THREADS;t++){
    printf("In main: creating thread %ld\n", t);
    rc = pthread_create(&threads[t], NULL, RDN, (void *)t);
    if (rc){
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
        }
    }
    pthread_exit(NULL);

}