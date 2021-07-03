#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX 100

float izracunPostotkaZaLokaciju(char** dvorana, int brRed, int brStup, int mjestoRed, int mjestoStupac, int prof, int profStrog);
float** izracunajProlaznosti(char** dvorana, int brRed, int brStup, int prof, int profStrog);
float nadjiNajboljeMjesto(float** prolaznost, int brRed, int brStup, int* najboljiRed, int* najboljiStupac);
int** labirint_izracun(char** dvorana, int brRed, int brStup, int x, int y);
int labirint_rekurzivna(char** dvorana, int brRed, int brStup, int x, int y, int i, int j, int** labirint);

int main()
{
    char **pozicije = NULL;
    char lab_izbor;
    float strogoca;
    float max;
    float **prolaz_x = NULL;
    int **labirint = NULL;
    int r,s;
    int x,y;
    int pozicija_p;
    int i,j;

    printf("Unesite velicinu dvorane:\n");

    scanf("%d%d",&r,&s);

    if((r < 3 || s < 3)){
        printf("Premala dvorana za ispit");
        exit(1);
    }
    else if((r > 20 || s > 20)){
        printf("Prevelika dvorana za ispit");
        exit(1);
    }

    pozicije = calloc(r+1,sizeof(int*));

    for(i = 0; i <= r; i++){
        pozicije[i] = calloc(s,sizeof(int));
    }

    printf("Unesite poziciju i strogocu profesora:\n");

    scanf("%d%f",&pozicija_p,&strogoca);

    if(pozicija_p > s){
        printf("Profesor mora biti u predavaonici!");
        exit(1);
    }

    printf("Unesite pozicije studenata u obliku %d*%d matrice sa pozicijama studenata oznacenima znakom 'X':\n", r,s);

    for(i = 1; i <= r; i++){
        for(j = 0; j < s; j++){
            scanf(" %c", &pozicije[i][j]);
        }
    }

    printf("Zelite li pokrenuti izracun(D,N)?");

    scanf(" %c", &lab_izbor);

    pozicije[0][pozicija_p-1] = 'P';

    prolaz_x = izracunajProlaznosti(pozicije,r,s,pozicija_p-1,strogoca);

    for(i = 0; i < r; i++,printf("\n")){
        for(j = 0; j < s; j++){
            printf("%6.2f", prolaz_x[i][j]);
        }
    }

    max = nadjiNajboljeMjesto(prolaz_x,r,s,&x,&y);

    printf("Najveci postotak je: %.2f%% na koordinatama X=%d, Y=%d", max,x,y);

    if(lab_izbor == 'D'){

        printf("\n");

        labirint_izracun(pozicije,r,s,x,y);

    }


    for(i = 0; i <= r+1; i++){
        for(j = 0; j < s; j++){
            free(pozicije);
        }
    }

    for(i = 0; i < r; i++){
        for(j = 0; j < s; j++){
            free(prolaz_x);
        }
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////


int** labirint_izracun(char** dvorana, int brRed, int brStup, int x, int y){

    int **labirint = NULL;
    int ulaz_i,ulaz_j;
    int i,j;

    labirint = calloc(brRed,sizeof(int*));

    for(i = 0; i < brRed; i++){
        labirint[i] = calloc(brStup,sizeof(int));
    }

    for(i = 1; i <= brRed; i++){
        for(j = 0; j < brStup; j++){
            if(dvorana[i][j] == 'X'){
                if((i == 1 || i == brRed) || (j == 0 || j == brStup-1)){
                    ulaz_i = i-1;
                    ulaz_j = j;
                }
            }
            else{
                dvorana[i][j] = 0;
            }

        }
    }

    labirint_rekurzivna(dvorana,brRed,brStup,x-1,y-1,ulaz_i,ulaz_j,labirint);

    for(i = 0; i < brRed; i++, printf("\n")){
        for(j = 0; j < brStup; j++){
            printf("%d", labirint[i][j]);
        }
    }



}

int labirint_rekurzivna(char** dvorana, int brRed, int brStup, int x, int y, int i, int j, int** labirint){

     if(i == x && j == y){

        labirint[i][j] = 1;

        return 1;
     }

     if(((i+1 >= 1 && i < brRed) && (j >= 0 && j < brStup)) && (labirint[i][j] == 0 && dvorana[i+1][j] == 'X')){

            labirint[i][j] = 1;


            if(labirint_rekurzivna(dvorana,brRed,brStup,x,y,i+1,j,labirint) == 1){

                return 1;

            }

            if(labirint_rekurzivna(dvorana,brRed,brStup,x,y,i-1,j,labirint) == 1){

                return 1;

            }

            if(labirint_rekurzivna(dvorana,brRed,brStup,x,y,i,j+1,labirint) == 1){

                return 1;

            }

            if(labirint_rekurzivna(dvorana,brRed,brStup,x,y,i,j-1,labirint) == 1){

                return 1;

            }

            labirint[i][j] = 0;
            return 0;

     }

     return 0;

}


/////////////////////////////////////////////////////////////////////////////////////////////


float nadjiNajboljeMjesto(float** prolaznost, int brRed, int brStup, int* najboljiRed, int* najboljiStupac){

    float tmp = 0.0;
    int i,j;

    for(i = 0; i < brRed; i++){
        for(j = 0; j < brStup; j++){
            if(prolaznost[i][j] > tmp){
                tmp = prolaznost[i][j];
                *najboljiRed = i+1;
                *najboljiStupac = j+1;
            }
        }
    }

    return tmp;

}

float** izracunajProlaznosti(char** dvorana, int brRed, int brStup, int prof, int profStrog){

    float **vrijednosti = 0;
    int i,j;

    vrijednosti = calloc(brRed,sizeof(int*));

    for(i = 0; i < brRed; i++){
        vrijednosti[i] = calloc(brStup,sizeof(int));
    }

    for(i = 1; i <= brRed; i++){
        for(j = 0; j < brStup; j++){
            if(dvorana[i][j] == 'X'){
                vrijednosti[i-1][j] = izracunPostotkaZaLokaciju(dvorana,brRed,brStup,i,j,prof,profStrog);
            }
        }
    }

        return vrijednosti;

}


float izracunPostotkaZaLokaciju(char** dvorana, int brRed, int brStup, int mjestoRed, int mjestoStupac, int prof, int profStrog){

    float strogoca;
    int a,b;
    int i,j;
    int n;
    float sanse = 0;

                if(mjestoStupac == prof){

                    strogoca = (float)profStrog/(mjestoRed*mjestoRed);

                }
                else if(mjestoStupac < prof){

                    a = prof - mjestoStupac;
                    b = mjestoRed;

                    if(mjestoRed < 2){
                        b = mjestoRed-1;
                    }

                    strogoca = profStrog/(sqrt((a*a)+(b*b)) * sqrt((a*a)+(b*b)));

                }
                else if(mjestoStupac > prof){

                    a = mjestoStupac - prof;
                    b = mjestoRed;

                    if(mjestoRed < 2){
                        b = mjestoRed-1;
                    }

                    strogoca = profStrog/(sqrt((a*a)+(b*b)) * sqrt((a*a)+(b*b)));

                }


                for(i = 1; i <= brRed; i++){
                    for(j = 0; j < brStup; j++){

                        if(dvorana[i][j] == 'Z'){

                                if(i == mjestoRed){

                                    if(j < mjestoStupac){

                                        n = mjestoStupac - j;

                                        sanse += (float)90/(n*n);
                                    }
                                    else if(j > mjestoStupac){

                                        n = j-mjestoStupac;

                                        sanse += (float)90/(n*n);
                                    }
                                }
                                else if(j == mjestoStupac){

                                    if(i < mjestoRed){

                                        n = mjestoRed - i;

                                        sanse += (float)90/(n*n);
                                    }
                                    else if(i > mjestoRed){
                                        n = i-mjestoRed;

                                        sanse += (float)90/(n*n);
                                    }

                                }else{

                                    if(i < mjestoRed){

                                        if(j < mjestoStupac){

                                            a = mjestoStupac - j;
                                            b = mjestoRed - i;

                                            sanse += (float)90/(sqrt((a*a)+(b*b)) * sqrt((a*a)+(b*b)));

                                        }
                                        else if(j > mjestoStupac){

                                            a = j - mjestoStupac;
                                            b = mjestoRed - i;

                                            sanse += (float)90/(sqrt((a*a)+(b*b)) * sqrt((a*a)+(b*b)));

                                        }

                                    }else if(i > mjestoRed){

                                        if(j < mjestoStupac){

                                            a = mjestoStupac - j;
                                            b = mjestoRed - i;

                                            sanse += (float)90/(sqrt((a*a)+(b*b)) * sqrt((a*a)+(b*b)));

                                        }
                                        else if(j > mjestoStupac){

                                            a = j - mjestoStupac;
                                            b = mjestoRed - i;

                                            sanse += (float)90/(sqrt((a*a)+(b*b)) * sqrt((a*a)+(b*b)));

                                        }

                                    }
                                }

                        }

                    }
                }




                for(i = 1; i <= brRed; i++){
                    for(j = 0; j < brStup; j++){

                           if(dvorana[i][j] == 'N'){

                                    if(i == mjestoRed){

                                        if(j < mjestoStupac){

                                            n = mjestoStupac - j;

                                            sanse -= (float)30/(n*n);
                                        }
                                        else if(j > mjestoStupac){

                                            n = j-mjestoStupac;

                                            sanse -= (float)30/(n*n);
                                        }

                                    }
                                    else if(j == mjestoStupac){

                                        if(i < mjestoRed){

                                            n = mjestoRed - i;

                                            sanse -= (float)30/(n*n);
                                        }
                                        else if(i > mjestoRed){

                                            n = i-mjestoRed;

                                            sanse -= (float)30/(n*n);
                                        }

                                    }else{

                                        if(i < mjestoRed){

                                            if(j < mjestoStupac){

                                                a = mjestoStupac - j;
                                                b = mjestoRed - i;

                                                sanse -= 30/pow(sqrt((a*a)+(b*b)),2);

                                            }
                                            else if(j > mjestoStupac){

                                                a = j - mjestoStupac;
                                                b = mjestoRed - i;

                                                sanse -= 30/pow(sqrt((a*a)+(b*b)),2);

                                            }

                                        }else if(i > mjestoRed){

                                            if(j < mjestoStupac){

                                                a = mjestoStupac - j;
                                                b = mjestoRed - i;

                                                sanse -= 30/pow(sqrt((a*a)+(b*b)),2);

                                            }
                                            else if(j > mjestoStupac){

                                                a = j - mjestoStupac;
                                                b = mjestoRed - i;

                                                sanse -= 30/pow(sqrt((a*a)+(b*b)),2);

                                            }

                                        }

                            }

                        }
                    }
                }

                sanse -= strogoca;

                if(sanse < 0.0){
                    sanse = 0.0;
                }
                else if(sanse > 100.0){
                    sanse = 100.0;
                }

                return sanse;

}
