#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <cmath>
#include "time.h"

using namespace std;

int cmpTTT = 0;   //compteur de comparation ColorExact
int cmpTTT1 = 0;  //compteur de comparation DSATUR
int nSommet = 35; // nombre de sommets
int kPeterson = 1;//initailisation basique
int nPeterson = 5;//initailisation basique

int colorationVoisin = 2;         //contrainte de la coloration pour les voisins de sommmet
int colorationVoisindeVoisin = 1; //contrainte de la coloration pour les voisins de voisins de sommmet

int *couleur1;       //[n];  // couleurs des sommets pour l'agorithme exact
int *couleur2;       //[n]; // couleurs pour DSATUR
int *DSAT;           //[n]; // degrés de saturation
int *Degre;          //[n]; // degrés des sommets
bool trouve = false; // permet de stopper l'algorithme exact
                     // quand une coloration  a ete trouvee

////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////// Fonction générale :
int isVoisin(int a, int b);            //vérifie si a et b sont voisin dans le graphe de Peterson
bool convient(int x, int c, int choix); // teste si la couleur c peut être donnee au sommet x en L(2,1)-coloration
int couleurMax(int tabCouleur[]);      //calcul le nombre de couleur d'un graphe coloré
void toStringGP(int *tab);             //affichage des couleurs du graphe de Peterson sous la forme demandé

////////////// Fonction ColorExact :
void ColorExact();          //méthode de test des coloration par brut force récursif
int ColorExactRecursif(int x);        // fonction recursive pour tester toutes les couleurs possible pour le sommet x

////////////// Fonction DSATUR :
int dsatMax();                         //saturation max du graphe
int DSATUR();                         //méthode de coloration en L(2,1) par la méthode de saturation des sommets

////////////// Fonction d'appel :
void comparaisonEnToutK(int kMaxComparaison); //Compare la methode ColorExact et DSATUR 
void menu();                           //menu principale présentant toute les fonctionnalités

////////////// debug et fonction générique :

void init();                           //initialisation de espaces mémoires réservés
void freeAll();                        //libère les espaces mémoires alloués
void afficheMatriceAdjacence();          //affiche la matrice d'adjacence

////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
  menu();//le menu initialise les espace alloué lui même
  freeAll();
  return 0;
}

////////////// Fonction générale :

int isVoisin(int a, int b)//test si a et b sont voisins dans le graphe de Peterson
{
  if (a < b){
    if (a < nPeterson){
      if (a + 1 == b && b < nPeterson || a + nPeterson == b || b == (nPeterson - 1) && a == 0)
      {
        return true;
      }
    }
    else{
      if (nPeterson + (a + kPeterson)%nPeterson == b  || nPeterson + (b + kPeterson)%nPeterson == a){
        return true;
      }
    }
  }
  else if (a > b){
    return isVoisin(b, a);
  }
  return false;
}

bool convient(int x, int c, int choix) // teste si la couleur c peut être donnee au sommet x (elle n'est pas utilisee par un de ses voisins)
{
  int nbSommetVerif=0;
  int *tabCouleur;
  if(choix == 1){
    tabCouleur = couleur1;
    cmpTTT++;
  }
  else{
    tabCouleur = couleur2;
    cmpTTT1++;
  }
  
  for (int i = 0; i < nSommet; i++)
  {
    if (isVoisin(x, i))
    {
      if (!(abs(tabCouleur[i] - c) >= colorationVoisin) && tabCouleur[i]!= 0)
      {
        return false;
      }
      for (int k = 0; k < nSommet; k++)
      {
        if (isVoisin(k, i) && !(abs(tabCouleur[k] - c) >= colorationVoisindeVoisin) && tabCouleur[k]!= 0)
        {
          return false;
        }
      }
    }
  }
  return true;
}

int couleurMax(int tabCouleur[])//calcul le nombre de couleur d'un graphe coloré
{
  int cmp = 0;
  int couleur = 0;
  int b = 0;
  for (int i = 0; i < nSommet; i++)
  {
    couleur = tabCouleur[i];
    b = 1;
    for (int j = 0; j < i; j++)
    {
      if (tabCouleur[j] == couleur)
      {
        b = 0;
      }
    }
    cmp += b;
  }
  return cmp;
}

void toStringGP(int *tab)//affichage des couleurs du graphe de Peterson sous une forme plus visuel
{ 
  cout << "n : " << nPeterson << " k : " << kPeterson << endl;
  for (int i = 0; i < nPeterson; i++)
  {
    cout << " -- " << tab[i];
  }
  cout << endl;
  for (int i = 0; i < nPeterson; i++)
  {
    cout << " -- " << tab[i + nPeterson];
  }
  cout << endl;
}

////////////// Fonction ColorExact :

void ColorExact() //méthode de test des coloration par brut force récursif
{
  ColorExactRecursif(0);
}
int ColorExactRecursif(int x) // fonction recursive pour tester toutes les couleurs possible pour le sommet x
{
  if (x == nSommet)
  {
    return 0;
  }
  else
  {
    for (int c = 1; c <= nSommet; c++)
    {
      if (convient(x, c, 1))
      {
        couleur1[x] = c;
        return ColorExactRecursif(x + 1);
      }
    }
    return 1;
  }
  return 1;
}
////////////// Fonction DSATUR :

int dsatMax() //saturation max du graphe
{
  int maxDeg = -1, maxDSAT = -1, smax = 0;
  for (int i = 0; i < nSommet; i++)
  {
    if (couleur2[i] == -1 && (DSAT[i] > maxDSAT || (DSAT[i] == maxDSAT && Degre[i] > maxDeg)))
    {
      maxDSAT = DSAT[i];
      maxDeg = Degre[i];
      smax = i;
    }
  }
  return smax;
}

int DSATUR() //méthode de coloration en L(2,1) par la méthode de saturation des sommets
{
  int c, x, cmax = 0;
    for (int i = 0; i < nSommet; i++)
  {
    couleur2[i] = -1;
    DSAT[i] = 0;
    Degre[i] = 0;
    for (int j = 0; j < nSommet; j++)
    {
      if (isVoisin(i, j))
      {
        Degre[i]++;
      }
    }
    DSAT[i] = Degre[i];
  }

  for (int nb = 0;nb < nSommet;nb++) // tant qu'on a pas colorié tous les sommets
  {
    c = 1;
    x = dsatMax(); // on choisit le sommet de DSAT max non encore colorié

    while (!convient(x, c, 2))
    {
      c++; // on cherche la plus petite couleur disponible pour ce sommet
      //cout << "g" << c;
    }
    for (int j = 0; j < nSommet; j++)
    { // mise à jour des DSAT
      if (isVoisin(x, j) && convient(j, c, 2))
      {
        DSAT[j]++; // j n'avait aucun voisin colorié c,on incrémente donc son DSAT
      }
    }
    couleur2[x] = c;
    if (cmax < c)
    {
      cmax = c;
    }
  }
  return cmax;
}


////////////// Fonction d'appel :

void comparaisonEnToutK(int kMaxComparaison) //Compare la methode ColorExact et DSATUR sur plusieur K
{
  int nbCouleurColoExact = 0;
  int nbCouleurDSATUR = 0;
  int scoreDSATUR = 0;
  int scoreColorExact = 0;
  int tempColorExact = 0;
  int tempDSATUR = 0;
  clock_t clkStart;
  clock_t clkStart1;
  clock_t clkFinish;
  clock_t clkFinish1;

  if (kMaxComparaison == 0)
  {
    kMaxComparaison = nPeterson;
  }
  for (kPeterson = 1; kPeterson <= min(((double)nPeterson / 2.0), (double)kMaxComparaison); kPeterson++)
  {
    clkStart = clock();
    ColorExact();
    clkFinish = clock();

    clkStart1 = clock();
    DSATUR();
    clkFinish1 = clock();

    nbCouleurColoExact += couleurMax(couleur1);
    nbCouleurDSATUR += couleurMax(couleur2);

    tempColorExact = clkFinish - clkStart;
    scoreColorExact += tempColorExact;

    tempDSATUR = clkFinish1 - clkStart1;
    scoreDSATUR += tempDSATUR;
  }
  cout << "" << nPeterson << ";" << nbCouleurColoExact << ";" << nbCouleurDSATUR << ";" << scoreColorExact << ";" << scoreDSATUR << ";" << cmpTTT << ";" << cmpTTT1 ;

}


void menu()//menu principale présentant toute les fonctionnalités
{
  int menu;
  cout << "/////" << endl;
  cout << "Voici le programme principale, il regroupe toute les fonctionnalites de base d'une L(2,1)-coloration sur un graphe de Peterson via ColorExact et DSATUR :" << endl;
  cout << "Menu :" << endl;
  cout << "1 : Methode ColorExact" << endl;
  cout << "2 : Methode DSATUR" << endl;
  cout << "3 : GP(n,k) pour tout k possible via ColorExact" << endl;
  cout << "4 : GP(n,k) pour tout k possible via DSATUR" << endl;
  cout << "5 : Exemple colorations n = 5,6,7,8" << endl;
  cout << "6 : Exemple colorations n= 30,40,50" << endl;
  cout << "7 : Combat entre ColorExact et DSATUR" << endl;
  cout << "Choix : ";
  cin >> menu;

  if (menu <= 4)
  {
    cout << "Avec en parametre n : " ;
    cin >> nPeterson;
  }
  init();
  if (menu < 3)
  {
    cout << "Avec en parametre k : ";
    cin >> kPeterson;
  }
  int resolutionCombat = 10;
  if (menu == 7)
  {
    cout << "Avec pour nombre de repetition k (de preference 20) :";
    cin >> resolutionCombat;
  }
  cout << "/////" << endl;
  switch (menu)
  {
  case 1:
  {
    ColorExact();
    toStringGP(couleur1);
    cout << "nombre de couleurs utilises : " << couleurMax(couleur1) << endl;
    break;
  }
  case 2:
  {
    DSATUR();
    toStringGP(couleur2);
    cout << "nombre de couleurs utilises : " << couleurMax(couleur2) << endl;
    break;
  }
  case 3:
  {
    for (kPeterson = 1; kPeterson < ((double)nPeterson / 2.0); kPeterson++)
    {
      ColorExact();
      toStringGP(couleur1);
    }
    break;
  }
  case 4:
  {
    for (kPeterson = 1; kPeterson < ((double)nPeterson / 2.0); kPeterson++)
    {
      DSATUR();
      toStringGP(couleur2);
    }
    break;
  }
  case 5:
  {
    for(nPeterson=5;nPeterson<=8;nPeterson++){
      for(kPeterson=1;kPeterson<=2;kPeterson++){
        init();
        ColorExact();
        DSATUR();
        cout << "En ColorExact" << endl;
        toStringGP(couleur1);
        cout << "En DSATUR" << endl;
        toStringGP(couleur2);
        freeAll();
      }
    }
    break;
  }
  case 6:
  {
    for(nPeterson=30;nPeterson<=50;nPeterson+=10){
      for(kPeterson=1;kPeterson<=10;kPeterson+=9){
        init();
        ColorExact();
        DSATUR();
        cout << "En ColorExact" << endl;
        toStringGP(couleur1);
        cout << "En DSATUR" << endl;
        toStringGP(couleur2);
        freeAll();
      }
    }
    break;
  }

  case 7:
  {
    cout<<"nPeterson;nbCouleurColorExact;nbCouleurDSATUR;tempsColorExact;tempsDSATUR;nbComparaisonColorExact;nbComparaisonDSATUR"<<endl;
    for (int i = 1; i <= 10; i++)
    {
      nPeterson = i * 100;
      init();
      cmpTTT = 0;
      cmpTTT1 = 0;
      comparaisonEnToutK(resolutionCombat);
      cout <<endl;
      }
   } break;
  }
  cout << "/////" << endl;
  cout << "Realise par CHUPIN Clement et CHIBILE Romuald" << endl;
  cout << "/////" << endl;
}


////////////// debug et fonction générique :

void init() //initialisation de espaces mémoires réservés
{
  nSommet = nPeterson * 2;
  couleur1 = new int[nSommet];
  couleur2 = new int[nSommet];
  DSAT = new int[nSommet];
  Degre = new int[nSommet];
}

void freeAll() //libère les espaces mémoires alloués
{

  free(couleur1);
  free(couleur2);
  free(DSAT);
  free(Degre);
}


void afficheMatriceAdjacence() //affiche la matrice d'adjacence
{
  for (int i = 0; i < nSommet; i++)
  {
    for (int j = 0; j < nSommet; j++)
    {
      cout << " - ";
      cout << isVoisin(i, j);
    }
    cout << endl;
  }
}


