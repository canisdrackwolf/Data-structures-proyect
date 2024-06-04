# Data-structures-proyect
Data structures proyect

required application: Aplicación requerida por lo que es importante intalar antes de ejecutar
  -windows_10_cmake_Release_graphviz-install-11.0.0-win64.exe
  
adjancency_Matrix: Archivo de texto que el profe nos suminsitro para el ejercicio con lo grafos
  -matriz_de_adyacencias_30x30.txt
  
src: Codigo c++ del proyeto 
  -Main.cpp
  
images: Donde se almacenarán las imagenes generados de los arboles binarios

Data structures proyect.dev  // proyecto para abrir con aplicación c++

Data structures proyect.exe // Permite ejecutar el software sin tener instalado copilador C++

************************************************************************************************

Esto es para editar
completamente explicando cada parte del codigo 



1. Debe tener un menú principal en donde se pueda escoger 2 opciones.

1.1.  Presentar ABB Autobalanceados.

1.2.  Presentar Grafos.

2. Cada opción del menú principal debe permitir lo siguiente:

2.1. El submenú para árboles tiene las siguientes opciones: 
            a. Ingrese la lista de nodos y el recorrido para construir el árbol.
            b. Mostrar de forma visual el árbol inicial (indica si el árbol es AVL o no).
            c. Convertir en  AVL (si es el caso) el árbol inicial y mostrar la visualización del árbol.
            d. Convertir en arbol Rojo y Negro el arbol inicial y mostrar la visualización del árbol.

2.2. el submenú para grafos debe llevar las siguientes opciones:
            a. Ingrese la matriz de adyacencias (debe leer si es un grafo dirigido ponderado o un grafo no dirigido ponderado) para construir el grafo.
            b. Mostrar la visualización del grafo inicial.
            c. Entra a un submenú (Elija un algoritmo para encontrar el camino más corto)
                                                  -  Dijkstra (utiliza el grafo inicial resuelve con el algoritmo y luego entrega la solución)
                                                  -  Floyd-Warshall (utiliza el grafo inicial resuelve con el algoritmo y luego entrega la solución)
            d. Entra a un submenú (Elija un algoritmo para encontrar árboles de peso mínimo)
                                                  - Kruskal (utiliza el grafo inicial resuelve con el algoritmo y luego entrega la solución)
                                                  - Prim (utiliza el grafo inicial resuelve con el algoritmo y luego entrega la solución)
3. debe tener la opción de retornar al menú después de mostrar el resultado.

4. el código debe estar subido a github y en el readme una explicación con gráficos de ejemplo de cada opción.
