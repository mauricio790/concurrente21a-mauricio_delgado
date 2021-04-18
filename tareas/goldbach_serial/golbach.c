#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include <errno.h>
#include <stdlib.h>

#define SIZE 100000

void sumasGolbach(int64_t num, int64_t* arregloSumas);
int64_t cantidadSumas(int64_t num,int64_t candtidadPrimos ,int64_t * arregloSumas);
void golbachPar(int64_t num,int64_t mostrarSumas ,int64_t* arregloSumas);
void golbachImpar(int64_t num,int64_t mostrarSumas ,int64_t* arregloSumas);
int64_t siguientePrimo(int64_t primo);
void imprimirResultado(int64_t* arregloSumas, int64_t tamanoArreglo
  , int64_t signo, bool es_par);

int main(){
    FILE* input = stdin;
    int64_t num = 0ll;
    int64_t *arregloSumas;  
    arregloSumas = (int64_t *)calloc(SIZE,sizeof(int64_t));

    while(fscanf(input,"%"PRId64,&num) == 1){
        sumasGolbach(num,arregloSumas);
    }

    return EXIT_SUCCESS;
}

void sumasGolbach(int64_t num,int64_t* arregloSumas){
    int mostrarSumas = 0;
    
    printf("%" PRId64 ": ", num);

    if(num < 0){
        mostrarSumas = 1;
        num *= -1;
    }

    if (num >= -5 && num <= 5) {
        printf("NA\n");
    }else 
        if(num%2 == 0){
            golbachPar(num,mostrarSumas,arregloSumas);
        } else{
            golbachImpar(num,mostrarSumas,arregloSumas);
        }
}
  


void golbachPar(int64_t num,int64_t mostrarSumas ,int64_t* arregloSumas){
    int64_t par1 = 2;
    int64_t par2 = 2;
    int64_t indice = 0;
    
   
    //Si se pasa de la mitad, da vuelta a los pares de la suma
    //entonces se pone hasta la mitad para evitar repetir pares inversos
    while (par1 <= (num/2)) {
        par2 = par1;
        //se evalúa el primer par con los siguientes primos
        while (par2 < num) {
            //se suman los pares para ver si la suma es exitosa y se agregan al arreglo
            if (par1 + par2 == num) {
                arregloSumas[indice] = par1;
                arregloSumas[indice+1] = par2;
                indice += 2; //se salta al siguiente par
            }
            par2 = siguientePrimo(par2);
        }
        par1 = siguientePrimo(par1);
    }
    imprimirResultado(arregloSumas, indice, mostrarSumas, true);
}


void golbachImpar(int64_t num,int64_t mostrarSumas ,int64_t* arregloSumas){
    int64_t indice = 0;
     int64_t primo1 = 2;
    int64_t primo2 = 2;
    int64_t primo3 = 2;

    // el primer primo llega a la mitad para no repetir sumas
    while (primo1 <= (num/2)) {
    primo2 = primo1;//no hay sumas con primos más pequeños que primo1
    //se suman todos los primos hasta num para verificar las sumas 
    while (primo2 < num) {
      primo3 = primo2; //no hay sumas con primos más pequeños que 2
      // el tercer numero cambia mientras el segundo se mantiene
      while (primo3 < num) {
        // si la suma da el numero de goldbach se agregan los numeros al arreglo
        if (primo1 + primo2 + primo3 == num) {
          arregloSumas[indice] = primo1;
          arregloSumas[indice+1] = primo2;
          arregloSumas[indice+2] = primo3;
          indice += 3;
        }
        primo3 = siguientePrimo(primo3);
      }
      primo2 = siguientePrimo(primo2);
    }
    primo1 = siguientePrimo(primo1);
  }
  imprimirResultado(arregloSumas, indice, mostrarSumas, false);
}


int64_t siguientePrimo(int64_t primo){
    bool hay_primo = false;
  int64_t divisor = 0;
  // si es 2, suma un 1 y lo retorna
  if (primo == 2) {
    return primo+=1;
  } else {
    // sumar dos a mi primo actual e intentar dividir ese numero entre nuemros
    // imapares, si llega a 1 es primo
    while (hay_primo == false) {
      primo += 2;
      divisor = primo - 2;
      while (primo != 1) {
        // dividir el numero hasta que alguno llegue a 1
        if (primo % divisor == 0) {
          break;
        } else {
          divisor -= 2;
        }
      }
      if (divisor == 1) {
        hay_primo = true;
      }
    }
  }
    return primo;
}

void imprimirResultado(int64_t* arregloSumas, int64_t tamanoArreglo
  , int64_t mostrarSuma, bool es_par) {
    int64_t cantidadSumas = 0;
    if (es_par == true) {
        cantidadSumas = tamanoArreglo / 2;
    } else {
        cantidadSumas = tamanoArreglo / 3;
    }
    // si el signo es positivo unicamente se imprimen la cantidad de sumas
    if (mostrarSuma == 0) {  // ej: 6: 1 sums
        printf("%" PRId64 " sums\n", cantidadSumas);
    } else {  // si es negativo se imprimen la cantidad de sumas y las sumas
        printf("%" PRId64 " sums:", cantidadSumas);
        // si es par el for va de 2 en 2
        if (es_par ==  true) {
            for (int64_t indice = 0; indice < tamanoArreglo; indice+=2) {
                printf(" %" PRId64 " + ", arregloSumas[indice]);
                printf("%" PRId64 "", arregloSumas[indice+1]);
                if (indice + 2 != tamanoArreglo) {
                    printf(",");
                }
            }
        } else {  // si es impar el for va de 3 en 3
            for (int64_t indice = 0; indice < tamanoArreglo; indice+=3) {
                printf(" %" PRId64 " + ", arregloSumas[indice]);
                printf(" %" PRId64 " + ", arregloSumas[indice+1]);
                printf("%" PRId64 "", arregloSumas[indice+2]);
                if (indice + 3 != tamanoArreglo) {
                   printf(",");
                }
            }
        }
        printf("\n");
    }
}
