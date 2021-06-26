/**
  @file golbach.c
  @brief imprime la cantidad de sumas que genera la conjetura de 
  números positivos y muestra la cantidad y las sumas de los negativos
  @author Mauricio Delgado Leandro
  @date 18/04/2021
*/

#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <pthread.h>
#include "queue.h"
#include <stdlib.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/random.h>
#include <unistd.h>

#define MAX_SUMAS 10000

typedef struct 
{
  int64_t* array;
  int64_t capacidad;
}dynamic_array_t;

typedef struct 
{
  size_t thread_count;
  size_t total_num;
  size_t id_num;
  queue_t cola_entrada;

  pthread_mutex_t can_access_shared;
  char** array_sumas;

}shared_data_t;


int analyze_arguments(int argc, char* argv[], shared_data_t* shared_data);
int create_threads(shared_data_t* shared_data);

void free_matrix(const size_t row_count, void** matrix);
void** create_matrix(size_t row_count, size_t col_count, size_t element_size);

void* sumas_golbach(void* data);
int64_t cantidad_sumas(int64_t num,int64_t candtidadPrimos ,dynamic_array_t * array_sumas);
void golbach_par(int64_t num,int64_t mostrar_sumas,shared_data_t* shared_data, size_t id_num);
void golbach_impar(int64_t num,int64_t mostrar_sumas,shared_data_t* shared_data, size_t id_num);
int64_t siguiente_primo(int64_t primo);

void imprimir_resultado(dynamic_array_t* array_sumas
  , int64_t mostrar_suma, bool es_par, shared_data_t* shared_data,size_t id_num); 

void append_dynamic_array(dynamic_array_t*, int64_t);


/**
 * @brief lee los números de la entrada estándar y ejecuta el método que encuentra las sumas
 * @returns retorna 0, indica que no hay error
 */
int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;

    FILE* input = stdin;
    int64_t num = 0ll;

   
    shared_data_t *shared_data =
      (shared_data_t*) calloc(1,sizeof(shared_data_t));
    
    if (shared_data) {
      //sem_init(&shared_data->can_access_shared, 0, 1);
      shared_data->thread_count = sysconf(_SC_NPROCESSORS_ONLN);
      shared_data->id_num = 0;
      shared_data->total_num = 0;
      error = analyze_arguments(argc, argv, shared_data);
      if (error == EXIT_SUCCESS) {
        error = queue_init(&shared_data->cola_entrada);
        if(error == EXIT_SUCCESS){
          while(fscanf(input,"%"PRId64,&num) == 1){
            queue_append(&shared_data->cola_entrada,num); 
            shared_data->total_num++;
          }
          ///Crear la matriz antes de procesar números
          shared_data->array_sumas = (char**) create_matrix(shared_data->total_num
            ,MAX_SUMAS , sizeof(char));
            
          create_threads(shared_data);

          //Imprime todos los resultados
          for (size_t index = 0; index < shared_data->total_num; ++index) {
            printf("%s\n", shared_data->array_sumas[index]);
  }
        }
        else {
          fprintf(stderr, "error: could not create buffer\n");
          error = 1;
        }
      }
    }



    free(shared_data->array_sumas);
    free(shared_data);
    
    return EXIT_SUCCESS;
}

/**
 * @brief método encargado de analizar argumentos y guardarlos en la memoria compartida
 * @param int argc la cantidad de argumentos con que se ejecutó el programa
 * @param char* argv[]* array_sumas, acá se almacenan los sumandos 
 * @param shared_data_t* shared_data array_sumas, donde se guardan los argumentos
 */
int analyze_arguments(int argc, char* argv[], shared_data_t* shared_data) {
  int error = 0;
  if (argc == 2) {
    if (sscanf(argv[1], "%zu", &shared_data->thread_count) != 1){
        fprintf(stderr, "error: invalid thread count capacity\n");
        error = 2;
    } 
  } else {
    fprintf(stderr, "usage: thread_count\n");
      error = 1;
  }
  return error;
}


/**
 * @brief crea los hilos y cada hilo ejecuta las sumas de golbach, luego les hace join
 * @param shared_data_t* shared_data array_sumas, se envía a cada hilo para que tengan acceso a lo mismo 
 */
int create_threads(shared_data_t* shared_data) {
  assert(shared_data);
  
  int error = EXIT_SUCCESS;

  pthread_t* threads = (pthread_t*) calloc(shared_data->thread_count
    , sizeof(pthread_t));

    if (threads) {
      for (size_t index = 0; index < shared_data->thread_count; ++index) {
        if (error == EXIT_SUCCESS) {
          if (pthread_create(&threads[index], /*attr*/ NULL, sumas_golbach
          ,  (void *)shared_data) == EXIT_SUCCESS) {
          } else {
            fprintf(stderr, "error: could not create thread %zu\n", index);
            error = 21;
            shared_data->thread_count = index;
            break;
          } 
        }
      }

      for (size_t index = 0; index < shared_data->thread_count; ++index) {
        pthread_join(threads[index], /*value_ptr*/ NULL);
      }

      free(threads);
    }

    return error;
}


/**
 * @brief método encargado de encontrar la cantidad de sumas y mostrarlas en caso de indicarlo
 * @param void* data, información que pueden acceder todos los hilos. De acá se toman los números para procesar
 */
void* sumas_golbach(void* data){
    int mostrar_sumas = 0;
    shared_data_t *shared_data = (shared_data_t *)data;

    int64_t num = 0;
    size_t id_num = 0;
    
    
    while(!queue_is_empty(&shared_data->cola_entrada)){
      
      pthread_mutex_lock(&shared_data->can_access_shared);
      num = queue_dequeue(&shared_data->cola_entrada);
      id_num = shared_data->id_num++; ///Toma el índice a procesar y lo incrementa al siguiente
      pthread_mutex_unlock(&shared_data->can_access_shared);

      sprintf(shared_data->array_sumas[id_num], "%" PRId64 ": ", num);
      
      if(num < 0){
        mostrar_sumas = 1;
        num *= -1;
      } 

      if (num >= -5 && num <= 5) {
        strcat(shared_data->array_sumas[id_num],"NA");
      }else if(num%2 == 0){
            golbach_par(num,mostrar_sumas,shared_data,id_num);
        } else {
            golbach_impar(num,mostrar_sumas,shared_data,id_num);
        }
    }
    return NULL;
}
  

/**
 * @brief método encargado de encontrar las sumas de goldbach de números pares
 * @param int64_t num, número leído de la entrada estándar
 * @param shared_data_t* shared_data, acá se almacenan las sumas ya con formato
 * @param size_t id_num, indica la fila de la matriz que corresponde a cada número
 */
void golbach_par(int64_t num,int64_t mostrar_sumas ,shared_data_t* shared_data, size_t id_num ){
    int64_t par1 = 2;
    int64_t par2 = 2;
    
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
                append_dynamic_array(array_sumas,par1); 
                append_dynamic_array(array_sumas,par2);
            }
            par2 = siguiente_primo(par2);
        }
        par1 = siguiente_primo(par1);
    }
    imprimir_resultado(array_sumas, mostrar_sumas, true, shared_data,id_num);
    free(array_sumas);
}

/**
 * @brief método encargado de encontrar las sumas de goldbach de números pares
 * @param int64_t num, número leído de la entrada estándar
 * @param shared_data_t* shared_data, acá se almacenan las sumas ya con formato
 * @param size_t id_num, indica la fila de la matriz que corresponde a cada número
 */
void golbach_impar(int64_t num,int64_t mostrar_sumas ,shared_data_t* shared_data,size_t id_num){
     int64_t primo1 = 2;
    int64_t primo2 = 2;
    int64_t primo3 = 2;

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
          append_dynamic_array(array_sumas,primo1);
          append_dynamic_array(array_sumas,primo2);
          append_dynamic_array(array_sumas,primo3);
        }
        primo3 = siguiente_primo(primo3);
      }
      primo2 = siguiente_primo(primo2);
    }
    primo1 = siguiente_primo(primo1);
  }
  imprimir_resultado(array_sumas,  mostrar_sumas, false,shared_data,id_num);
   free(array_sumas);
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

/**
 * @brief método encargado de mostrar la cantidad de sumas de todos los números ingresados y además
 * muestra la suma de los números indicados
 * @param shared_data_t* shared_data, acá se almacenan las sumas ya con formato
 * @param int64_t tamanoArreglo, contiene el tamaño del arreglo
 * @param int64_t mostrarSuma, indicador de si se muestran o no las sumas
 * @param bool sses_par, indica si el número es par o no
 * 
 */
void imprimir_resultado(dynamic_array_t* array_sumas
  , int64_t mostrar_suma, bool es_par, shared_data_t* shared_data,
  size_t id_num) {
    int64_t cantidad_sumas = 0;
    char *a_concatenar = (char*) calloc(MAX_SUMAS,sizeof(char));
    if (es_par == true) {
        cantidad_sumas = array_sumas->capacidad / 2;
    } else {
        cantidad_sumas = array_sumas->capacidad / 3;
    }
    // si el signo es positivo unicamente se imprimen la cantidad de sumas
    if (mostrar_suma == 0) {  // ej: 6: 1 sums
      sprintf(a_concatenar,"%" PRId64 " sums", cantidad_sumas);
       strcat(shared_data->array_sumas[id_num],a_concatenar);
    } else {  // si es negativo se imprimen la cantidad de sumas y las sumas
        sprintf(a_concatenar,"%" PRId64 " sums:", cantidad_sumas);
        strcat(shared_data->array_sumas[id_num],a_concatenar);
        // si es par el for va de 2 en 2
        if (es_par ==  true) {
            for (int64_t indice = 0; indice < array_sumas->capacidad; indice+=2) {
                sprintf(a_concatenar," %" PRId64 " + ", array_sumas->array[indice]);
                strcat(shared_data->array_sumas[id_num],a_concatenar);
                sprintf(a_concatenar,"%" PRId64 "", array_sumas->array[indice+1]);
                strcat(shared_data->array_sumas[id_num],a_concatenar);
                if (indice + 2 != array_sumas->capacidad) {
                    strcat(shared_data->array_sumas[id_num],",");
                }
            }
        } else {  // si es impar el for va de 3 en 3
            for (int64_t indice = 0; indice < array_sumas->capacidad; indice+=3) {
              
              sprintf(a_concatenar," %" PRId64 " + ", array_sumas->array[indice]);
              strcat(shared_data->array_sumas[id_num],a_concatenar);
              sprintf(a_concatenar,"%" PRId64 " + ", array_sumas->array[indice+1]);
              strcat(shared_data->array_sumas[id_num],a_concatenar);
              sprintf(a_concatenar,"%" PRId64 "", array_sumas->array[indice+2]);
              strcat(shared_data->array_sumas[id_num],a_concatenar);
              if (indice + 3 != array_sumas->capacidad) {
                strcat(shared_data->array_sumas[id_num],",");
              }
            }
        }
    }
}


/**
 * @brief código tomado de 
 * http://jeisson.ecci.ucr.ac.cr/concurrente/2021a/ejemplos/source.php?file1=pthreads/hello_order_cond_safe/hello_order_cond_safe.c
 */
void** create_matrix(size_t row_count, size_t col_count, size_t element_size) {
  void** matrix = (void**) calloc(row_count, sizeof(void*));
  if ( matrix == NULL ) {
    return NULL;
  }

  for (size_t row = 0; row < row_count; ++row) {
    if ( (matrix[row] = calloc(col_count, element_size) ) == NULL ) {
      free_matrix(row_count, matrix);
      return NULL;
    }
  }

  return matrix;
}

/**
 * @brief código tomado de 
 * http://jeisson.ecci.ucr.ac.cr/concurrente/2021a/ejemplos/source.php?file1=pthreads/hello_order_cond_safe/hello_order_cond_safe.c
 */
void free_matrix(const size_t row_count, void** matrix) {
  if (matrix) {
    for (size_t row = 0; row < row_count; ++row) {
      free(matrix[row]);
    }
  }

  free(matrix);
}

void append_dynamic_array(dynamic_array_t* array_dinamico, int64_t num){
  
  int64_t new_capacity = array_dinamico->capacidad + 1;

  int64_t* nuevo_arreglo = (int64_t*) 
    realloc(array_dinamico->array,new_capacity*sizeof(int64_t));

  array_dinamico->array = nuevo_arreglo;
  array_dinamico->capacidad = new_capacity;
  array_dinamico->array[array_dinamico->capacidad - 1] = num;
}