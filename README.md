# Planificador de Algoritmos

Este proyecto educativo busca ayudar a entender los conceptos básicos de cómo funciona la administración de los recursos del CPU mediante un planificador de algoritmos representado por
el clásico **diagrama de Gant**. Así mismo, facilitar la visualización, seguimiento de un algoritmo, y entender cómo la CPU puede distribuir los recursos y destinarlos de forma eficiente.

## Algoritmos implementados

- **FIFO (First In, First Out):**
  
  Este algoritmo asigna el tiempo de procesamiento a los procesos en el orden en que llegan al sistema. Es fácil de implementar, pero puede resultar en un alto tiempo de espera para procesos cortos y un bajo
  uso del procesador para procesos largos.
  
- **SJF (Shortest Job First):**
  
  Este algoritmo selecciona el proceso que tiene el tiempo de ejecución más corto. Es eficiente y minimiza el tiempo de espera, pero puede bloquear procesos largos si llegan nuevos procesos que son más cortos.
  
- **SRTF (Shortest Remaining Time First):**
  
  Similar al SJF, pero considera el tiempo restante de ejecución de cada proceso. Si un nuevo proceso llega y tiene menos tiempo restante, se le asigna la CPU, lo que puede resultar en inanición si hay procesos
  largos que no se ejecutan.
  
- **Round Robin:**
  
  Este algoritmo reparte el tiempo de CPU en rodajas, asignando cada proceso un ciclo de ejecución en el orden en que llega. Es expropiativo y no utiliza prioridades, lo que puede resultar en un alto tiempo de
  espera para procesos cortos.
  
## Compilación y ejecución

1. Crear el directorio `build` y entrar:
   
   ```bash
   mkdir build && cd build
   ```
   
2. Generación y compilación:

   ```bash
   cmake ..
   cmake --build .
   ```
3. Ejecución:

   ```bash
   ./main
   ```

## Requerimientos:

- Raylib
- Cmake

De no contar con la librería requerida, continuar normalmente con la compilación, el archivo cmake se ocupará de integrar las dependencias necesarias de forma local en el entorno del proyecto.
