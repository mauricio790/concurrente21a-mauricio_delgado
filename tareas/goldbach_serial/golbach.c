#include<stdio.h>
#include<stdlib.h>


void sumasGolbach(long long);
void primos(long long num,long long *vecPrimos);
int cantidadSumas(long long num, FILE* output);
int esPrimo(long long num, long long *vecPrimo);
int main(){
    FILE* input = stdin;
    long long num = 0ll;

    while(fscanf(input,"%lld",&num) == 1){
        sumasGolbach(num);
    }


    return EXIT_SUCCESS;
}

void sumasGolbach(long long num){
    FILE* output = stdout;
    int mostrarSumas = 0;

    if(num < 0){
        mostrarSumas = 1;
        num *= -1;
    }

    
    if(mostrarSumas == 0){
        fprintf(output, "%lld: %d sums\n",num,cantidadSumas(num,output));
    } else {
        fprintf(output, "-%lld: %d sums: ",num,cantidadSumas((num*-1),output));
    }
    
    
    
}

int cantidadSumas(long long num, FILE* output){
    int cantidad = 0;
    int mostrarSumas = 0;
    long long *vecPrimos;
    long long par1, par2;

    output = stdout;

    vecPrimos = (long long *)calloc(num,sizeof(long long));
    
    if(num < 0){
        mostrarSumas = 1;
        num *= -1;
        
    }

    primos(num,vecPrimos);
    if(num % 2 == 0){
        for(par2 = vecPrimos[0]; par2>0; par2 = vecPrimos[par2 +1 ]){
            par1 = num-par2;
            if(esPrimo(num-par2,vecPrimos) == 1){
                cantidad++;
            }
            if (mostrarSumas == 1){
                fprintf(output,"%lld + %lld,", par1 , par2);     
            }
        }
    }
    

    free(vecPrimos);
    return cantidad;
}

int esPrimo(long long num, long long *vecPrimo){
    int esPrimo = 0;
    int indicePrimo = 0;
    while(esPrimo == 0 && indicePrimo != 0){
        if(vecPrimo[indicePrimo]==num){
            esPrimo = 1;
        }
    }
    
    return esPrimo;
}

void primos(long long num, long long *vecPrimos){

    long long contadorPrimo = 0;
        
    for(long long value = 2ll; value < num; value++){
        long long multiplo = 2;
        int esPrimo = 1;
        while(esPrimo == 1 && multiplo < value){
            if(value % multiplo == 0){
                esPrimo = 0;
            }
            multiplo++;
        }
        if(esPrimo == 1){
            vecPrimos[contadorPrimo] = value;
        }
    }
}