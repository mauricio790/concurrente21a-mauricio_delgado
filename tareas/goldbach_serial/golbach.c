/**
  @file golbach.c
  @brief imprime la cantidad de sumas que genera la conjetura de 
  números positivos y muestra la cantidad y las sumas de los negativos
  @author Mauricio Delgado Leandro
  @date 18/04/2021
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include <errno.h>
#include <stdlib.h>

typedef struct 
{
  int64_t* array;
  int64_t capacidad;
}dynamic_array_t;

void sumas_golbach(int64_t num, bool* primos, int64_t mayor);

void golbach_par(int64_t num,int64_t mostrar_sumas, bool* primos , int64_t mayor);
void golbach_impar(int64_t num,int64_t mostrar_sumas, bool* primos , int64_t mayor);
int64_t siguiente_primo(int64_t primo);
void imprimir_resultado(dynamic_array_t* array_sumas
  , int64_t mostrar_suma, int64_t cantidad_sumas, bool es_par); 
void append_dynamic_array(dynamic_array_t*, int64_t);
int64_t siguiente(int64_t primo, bool* primos, int64_t mayor);


/**
 * @brief lee los números de la entrada estándar y ejecuta el método que encuentra las sumas
 * @returns retorna 0, indica que no hay error
 */
int main(){
    FILE* input = stdin;
    int64_t num = 0ll;
    int64_t mayor = 0;
    dynamic_array_t* array_entrada = (dynamic_array_t*) calloc(1,sizeof(dynamic_array_t));
    //array_entrada->array = (int64_t*) calloc(1,sizeof(int64_t));
    array_entrada->capacidad = 0;

    while(fscanf(input,"%"PRId64,&num) == 1){
        append_dynamic_array(array_entrada,num);
        if(num<0){
          num *= -1;
        }
        if(num > mayor){
          mayor = num;
        }
    }


    bool* primos = (bool*) calloc(mayor + 1,sizeof(bool));
     for(int index = 0; index < mayor; index++){
              primos[index] = true;
     }
    
     //Criba
          for(int64_t divisor = 2; divisor<mayor + 1;divisor++){
            if(primos[divisor-1]==true ){
              for(int64_t multiplos = (divisor * divisor); multiplos<mayor + 1;multiplos+=divisor){
                primos[multiplos-1] = false;
                printf("%" PRId64 " es primo\n",multiplos-1);
              }
            }
          }
          //Criba
    
    for(int elemento = 0; elemento<array_entrada->capacidad;elemento++){
      sumas_golbach(array_entrada->array[elemento],primos,mayor);
      
    }

    free(primos);
    free(array_entrada);
    //free(array_sumas);
    
    return EXIT_SUCCESS;
}


/**
 * @brief método encargado de encontrar la cantidad de sumas y mostrarlas en caso de indicarlo
 * @param int64_t num, número leído de la entrada estándar
 * @param int64_t* array_sumas, acá se almacenan los sumandos 
 */
void sumas_golbach(int64_t num, bool* primos, int64_t mayor){
    int mostrar_sumas = 0;
    
    printf("%" PRId64 ": ", num);

    if(num < 0){
        mostrar_sumas = 1;
        num *= -1;
    }

    if (num >= -5 && num <= 5) {
        printf("NA\n");
    }else 
        if(num%2 == 0){
            golbach_par(num,mostrar_sumas, primos, mayor);
        } else{
            golbach_impar(num,mostrar_sumas, primos, mayor);
        }
}
  

/**
 * @brief método encargado de encontrar las sumas de goldbach de números pares
 * @param int64_t num, número leído de la entrada estándar
 * @param dynamic_array_t* array_sumas, acá se almacenan los sumandos
 */
void golbach_par(int64_t num,int64_t mostrar_sumas, bool* primos, int64_t mayor){
    int64_t par1 = 2;
    int64_t par2 = 2;

    int64_t cantidad_sumas = 0;

    dynamic_array_t* array_sumas;
    array_sumas = (dynamic_array_t*) calloc(1,sizeof(dynamic_array_t));
    array_sumas->capacidad = 0;
   
    //Si se pasa de la mitad, da vuelta a los pares de la suma
    //entonces se pone hasta la mitad para evitar repetir pares inversos
    while (par1 <= (num/2)) {
        par2 = par1;
        //se evalúa el primer par con los siguientes primos
        while (par2 < num) {
            //se suman los pares para ver si la suma es exitosa y se agregan al arreglo
            if (par1 + par2 == num) {
                if(mostrar_sumas == 1){
                  append_dynamic_array(array_sumas,par1); 
                  append_dynamic_array(array_sumas,par2);
                }
                cantidad_sumas++;
            }
            par2 = siguiente(par2,primos,mayor);
        }
        par1 = siguiente(par1,primos,mayor);
    }
    imprimir_resultado(array_sumas, mostrar_sumas, cantidad_sumas, true);
    free(array_sumas);
}

/**
 * @brief método encargado de encontrar las sumas de goldbach de números impares
 * @param int64_t num, número leído de la entrada estándar
 * @param dynamic_array_t* array_sumas, acá se almacenan los sumandos
 */
void golbach_impar(int64_t num,int64_t mostrar_sumas, bool* primos, int64_t mayor){
    int64_t primo1 = 2;
    int64_t primo2 = 2;
    int64_t primo3 = 2;

    int64_t cantidad_sumas = 0;

    dynamic_array_t* array_sumas;
    array_sumas = (dynamic_array_t*) calloc(1,sizeof(dynamic_array_t));
    array_sumas->capacidad = 0;

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
          if(mostrar_sumas == 1){
            append_dynamic_array(array_sumas,primo1);
            append_dynamic_array(array_sumas,primo2);
            append_dynamic_array(array_sumas,primo3);
          }
          cantidad_sumas++;
        }
        primo3 = siguiente(primo3,primos,mayor);
      }
      primo2 = siguiente(primo2,primos,mayor);
    }
    primo1 = siguiente(primo1,primos,mayor);
  }
  imprimir_resultado(array_sumas, mostrar_sumas, cantidad_sumas, false);
   free(array_sumas);
}


int64_t siguiente(int64_t primo, bool* primos,int64_t mayor){
  
  int64_t siguiente_primo = primo;
  
  bool hay_primo = true;
  while (siguiente_primo < mayor && hay_primo ){
    if(primos[siguiente_primo] == true)
      hay_primo = false;
    else 
      siguiente_primo++;
  }

  return siguiente_primo + 1;
  
}

/**
 * @brief calcula el siguiente número primo
 * @param int64_t número primo actual
 * @return int64_t retorna el siguiente número primo
 */
int64_t siguiente_primo(int64_t primo){
    bool hay_primo = false;
  int64_t divisor = 0;
  // si es 2, suma un 1 y lo retorna
  if (primo == 2) {
    return primo+=1;
  } else {
    // sumar dos a mi primo actual e intentar dividir ese numero entre nuemros
    // imapares, si llega a 1 es primo
    while (hay_primo == false) {
      divisor = primo;
      primo += 2;
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

/**
 * @brief método encargado de mostrar la cantidad de sumas de todos los números ingresados y además
 * muestra la suma de los números indicados
 * @param dynamic_array_t* array_sumas, arreglo que contendrá las sumas
 * @param int64_t tamanoArreglo, contiene el tamaño del arreglo
 * @param int64_t mostrarSuma, indicador de si se muestran o no las sumas
 * @param bool sses_par, indica si el número es par o no
 */
void imprimir_resultado(dynamic_array_t* array_sumas
  , int64_t mostrar_suma, int64_t cantidad_sumas, bool es_par) {

    // si el signo es positivo unicamente se imprimen la cantidad de sumas
    if (mostrar_suma == 0) {  // ej: 6: 1 sums
        printf("%" PRId64 " sums\n", cantidad_sumas);
    } else {  // si es negativo se imprimen la cantidad de sumas y las sumas
        printf("%" PRId64 " sums:", cantidad_sumas);
        // si es par el for va de 2 en 2
        if (es_par ==  true) {
            for (int64_t indice = 0; indice < array_sumas->capacidad; indice+=2) {
                printf(" %" PRId64 " + ", array_sumas->array[indice]);
                printf("%" PRId64 "", array_sumas->array[indice+1]);
                if (indice + 2 != array_sumas->capacidad) {
                    printf(",");
                }
            }
        } else {  // si es impar el for va de 3 en 3
            for (int64_t indice = 0; indice < array_sumas->capacidad; indice+=3) {
                printf(" %" PRId64 " + ", array_sumas->array[indice]);
                printf("%" PRId64 " + ", array_sumas->array[indice+1]);
                printf("%" PRId64 "", array_sumas->array[indice+2]);
                if (indice + 3 != array_sumas->capacidad) {
                   printf(",");
                }
            }
        }
        printf("\n");
    }
}

void append_dynamic_array(dynamic_array_t* array_dinamico, int64_t num){
  
  int64_t new_capacity = array_dinamico->capacidad + 1;

  int64_t* nuevo_arreglo = (int64_t*) 
    realloc(array_dinamico->array,new_capacity*sizeof(int64_t));

  array_dinamico->array = nuevo_arreglo;
  array_dinamico->capacidad = new_capacity;
  array_dinamico->array[array_dinamico->capacidad - 1] = num;
 
}
