#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <cmath>
#include "time.h"

using namespace std;


int nSommet = 35; // nombre de sommets
int kPeterson = 1;
int nPeterson = 5;
int degreMax = 3;

int colorationVoisin = 2;
int colorationVoisindeVoisin = 1;

int *couleur1;       //[n];  // couleurs des sommets pour l'agorithme exact
int *couleur2;
int **voisin ; //stockage des voisins dans un tableau [n][degréMax] 




////////////// Nouvelles fonctions :

////////////// Fonctions de voisinage
void initVoisin();
bool convientOpti(int x, int c);


////////////// Fonction ColorExactOpti :
void ColorExactOpti();
int ColorExactRecursifOpti(int x);


////////////// Anciennes fonctions :

////////////// Fonction ColorExact :
void ColorExact();                    //méthode de test des coloration par brut force récursif
int ColorExactRecursif(int x);        // fonction recursive pour tester toutes les couleurs possible pour le sommet x


////////////// Fonction générale :
int isVoisin(int a, int b);            //vérifie si a et b sont voisin dans le graphe de Peterson
bool convient(int x, int c); // teste si la couleur c peut être donnee au sommet x en L(2,1)-coloration
int couleurMax(int tabCouleur[]);      //calcul le nombre de couleur d'un graphe coloré
void toStringGP(int *tab);             //affichage des couleurs du graphe de Peterson sous la forme demandé

////////////// debug et fonction générique :
void init();                           //initialisation de espaces mémoires réservés
int comparaison();                     //compare le résultat des 2 algorithme
void freeAll();                        //libère les espaces mémoires alloués



////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
  nPeterson=5;
  kPeterson=2;
  init();
  initVoisin();
  clock_t clkStart;
  clock_t clkStart1;
  clock_t clkFinish;
  clock_t clkFinish1;
  int cmp=0,cmp1=0,diff=0;
  cout << "//////" << endl;
  cout << "Programme comparant ColorExact a sa version optimisee en temps de calcul et en coloration obtenu" << endl;
  cout << "//////" << endl;
  cout << "nPeterson ; Temps_ColorExact ; Temps_ColorExact_opti ; difference entre les 2 colorations" << endl;
  for(int i=1;i<=10;i++){
    for(kPeterson=1;kPeterson<40;kPeterson++){
      nPeterson=i*100;
      freeAll();
      init();
      initVoisin();

      clkStart = clock();
      ColorExact();
      //toStringGP(couleur1);
      clkFinish = clock();
      cmp+=clkFinish - clkStart;

      clkStart1 = clock();
      ColorExactOpti();
      //toStringGP(couleur2);
      clkFinish1 = clock();
      cmp1+=clkFinish1 - clkStart1;

      diff+=comparaison();
    }
    cout << "" << nPeterson << ";" << cmp << ";" << cmp1 << ";" << diff << endl;
    cmp=0,cmp1=0,diff=0;
 }
  cout << "/////" << endl;
  cout << "Realise par CHUPIN Clement et CHIBILE Romuald" << endl;
  cout << "/////" << endl;
  return 0;
}

int comparaison(){
  int cmpt=0;
  for(int i=0;i<nSommet;i++){
    cmpt+=abs(couleur1[i]-couleur2[i]);
  }
  return cmpt;
}

//////////////////// Nouvelles fonctions :

void initVoisin(){
    for(int i=0;i<nSommet;i++){
        for(int j=0;j<degreMax;j++){
            voisin[i][j]=0;
        }
    }

    for(int i=0;i<nPeterson;i++){
      voisin[i][0]=(i+1)%nPeterson;
      voisin[i][1]=i+nPeterson;
      voisin[i][2]=(i+nPeterson-1)%nPeterson;
    }
    for(int i=0;i<nPeterson;i++){
      voisin[nPeterson+i][0]=i;
      voisin[nPeterson+i][1]=nPeterson+(i+kPeterson)%nPeterson;
      voisin[nPeterson+i][2]=nPeterson+(nPeterson+i-kPeterson)%nPeterson;
    }


}


bool convientOpti(int x, int c) // teste si la couleur c peut être donnee au sommet x (elle n'est pas utilisee par un de ses voisins)
{
  int voisinV=0,voisinDeVoisinV=0;
  for (int i = 0; i < degreMax; i++)
  {
    voisinV=voisin[x][i];
    if (!(abs(couleur2[voisinV] - c) >= colorationVoisin) && couleur2[voisinV]!= 0)
    {
      return false;
    }
    for (int k = 0; k < degreMax; k++)
    {
      voisinDeVoisinV=voisin[voisinV][k];
      if (!(abs(couleur2[voisinDeVoisinV] - c) >= colorationVoisindeVoisin) && couleur1[voisinDeVoisinV]!= 0)
      {
      return false;
     }
    }
  }
  return true;
}

////////////// Fonction ColorExactOpti :

void ColorExactOpti() //méthode de test des coloration par brut force récursif
{
  ColorExactRecursifOpti(0);
}
int ColorExactRecursifOpti(int x) // fonction recursive pour tester toutes les couleurs possible pour le sommet x
{
  if (x == nSommet)
  {
    return 0;
  }
  else
  {
    for (int c = 1; c <= nSommet; c++)
    {
      if (convientOpti(x, c))
      {
        couleur2[x] = c; //cout << "=>couleur de " << x << " : " << couleur[x] << endl;
        return ColorExactRecursifOpti(x + 1);
      }
    }

    return 1;
  }
  return 1;
}




////////////// Anciennes fonctions :

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

bool convient(int x, int c) // teste si la couleur c peut être donnee au sommet x (elle n'est pas utilisee par un de ses voisins)
{
  for (int i = 0; i < nSommet; i++)
  {
    if (isVoisin(x, i))
    {
      if (!(abs(couleur1[i] - c) >= colorationVoisin) && couleur1[i]!= 0)
      {
        return false;
      }
      for (int k = 0; k < nSommet; k++)
      {
        if (isVoisin(k, i) && !(abs(couleur1[k] - c) >= colorationVoisindeVoisin) && couleur1[k]!= 0)
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
    //toStringGP(couleur1);
    return 0;
  }
  else
  {
    for (int c = 1; c <= nSommet; c++)
    {
      if (convient(x, c))
      {
        couleur1[x] = c;
        return ColorExactRecursif(x + 1);

      }
    }
    return 1;
  }
  return 1;
}

void init() //initialisation de espaces mémoires réservés
{
  nSommet = nPeterson * 2;
  couleur1=(int*)malloc(nSommet*sizeof(int));
  couleur2=(int*)malloc(nSommet*sizeof(int));
  voisin = new int*[nSommet];

  for(int i=0;i<nSommet;i++){
      voisin[i] = new int[degreMax];
      couleur1[i]=0;
      couleur2[i]=0;
  }
}
void freeAll() //initialisation de espaces mémoires réservés
{
  for(int i=0;i<nSommet;i++){
    free(voisin[i]);
  }
  free(voisin);
  free(couleur1);
  free(couleur2);
}