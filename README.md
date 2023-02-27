# Reseau-de-neurones-en-C-

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
