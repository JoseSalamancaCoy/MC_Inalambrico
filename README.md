# MC_Inalambrico
Este proyecto contiene lo necesario para implementar un monitor cardíaco inálambrico para Ratas

**Actualmente tienen tres módulos:**

- 1) Adquisición (módulo AD8232, amplificador de la señal del corazón)

- 2) Procesamiento (la tarjeta que se diseñó en la versión anterior) que básicamente binarisa la señal del corazón y la empaqueta para la transmisión

- 3) Transmisión (actualmente con la xbee). 


Inicialmente no habían problemas (no se sabía realmente a lo que se enfrentaban y eso es normal)   sin embargo hoy con la experiencia y las sugerencias de sus directores entonces tienen problemas en los 3 módulos. 

**Problemas:**

- 1) Adquisición: el ancho de banda del módulo genérico está bien para la señal humana pero no para las ratas, hay que modificar el filtro de salida o de entrada (no lo tengo claro cuál se debe modificar) para que funcione perfectamente con la señal del corazón de las ratas. 

- 2) Procesamiento: acá el lío que yo le veo es que es un espacio innesazario por qué existen otras soluciones que pueden resumir en un encapsulado la adquisición y la transmisión.

- 3) Transmisión: Para la señal del corazón los 256Kbytes de la xbee funcionan, pero como la señal del corazón de la rata es más rápida entonces hay que muestrear más rápido y transmitir más cantidad de datos entonces el ancho de banda está al límite y se comienza a quedar corto entonces no es muy confiable, adicionalmente la xbee es una tarjeta de más que está ocupando espacio (vertical)


**Solucion**

Reemplazar procesamiento y transmisión por una ESP32, con ella hacer 2 y 3 (Cambiar xbee a bluetooth ya que el ancho de banda es de 2MHz) de esa forma se resume a una sola tarjeta la adquisición y la transmisión.
