
1. Las líneas de seguridad en el archivo .h

Para que el compilador no proteste si olvidas poner el flag -D, se utilizan tres directivas del preprocesador. Estas directivas actúan como un condicional antes de que el código se traduzca a binario.
En tu archivo get_next_line.h, debes colocar de forma consecutiva la siguiente lógica (utilizando las palabras clave del preprocesador que empiezan con #):
1. La condición (#ifndef): Significa "if not defined" (si no está definido). Le preguntas al preprocesador: ¿Alguien ha definido la palabra BUFFER_SIZE al compilar?
2. La definición (#define): Si la respuesta a la pregunta anterior es "no" (es decir, no se usó el flag -D), esta línea entra en acción y crea la macro asignándole un valor por defecto (por convenio en 42 se suele poner 42 o 1).
3. El cierre (#endif): Cierra el bloque condicional del preprocesador.
Al meter este bloque en tu cabecera, si compilas con -D BUFFER_SIZE=10, el preprocesador ignorará el bloque del .h porque ya está definido. Pero si compilas a secas sin el flag, el .h saltará al rescate y le asignará el valor por defecto que tú hayas elegido.

2. El bucle while de lectura (Justo después del escudo)

Una vez que tu función pasa el escudo que pusiste (if (fd < 0 || BUFFER_SIZE <= 0)), necesitas meterte en el bucle que va a rellenar tu variable estática (stash).
Para plantear este while, necesitas controlar dos condiciones a la vez. El bucle debe seguir ejecutándose mientras se cumpla lo siguiente:
• Condición A (El archivo tiene texto): Tienes que guardar el resultado de la función read() en una variable (por ejemplo, bytes_leidos). El bucle debe continuar mientras bytes_leidos sea mayor que 0. Si es 0, significa que has llegado al final del archivo (EOF) y hay que parar.
• Condición B (Aún no tenemos una línea completa): No quieres leer todo el archivo de golpe si es enorme. Quieres parar en cuanto tengas una línea lista. Por lo tanto, el bucle debe continuar mientras tu función de buscar (el strchr tuneado) NO encuentre un \n dentro de tu variable estática.

¿Cómo interactúa esto dentro del bucle?

1. Antes de entrar al bucle: Inicializas tu variable de control (por ejemplo, bytes_leidos = 1) para asegurarte de que el bucle arranca la primera vez.
2. La condición del while: Evalúa que bytes_leidos > 0 Y que no haya un \n en tu estática.
3. Dentro del bucle:
	• Llamas a read() pasándole tu buffer temporal y el BUFFER_SIZE.
	• Actualizas bytes_leidos con lo que devuelva read().
	• Si bytes_leidos es -1, significa que ha habido un error físico de lectura. Tienes que liberar inmediatamente la estática, ponerla a NULL y salir con un return (NULL) para evitar fugas de memoria.
	• Si todo va bien, le pones el nulo al final al buffer temporal (buffer[bytes_leidos] = '\0').
	• Fusionas el buffer temporal con tu variable estática usando tu función de unir (strjoin tuneado), la cual debe liberar la memoria vieja de la estática en cada paso.
En cuanto el read llegue al final del archivo (devuelva 0) o tu función de buscar detecte un \n en la estática, el bucle while se romperá automáticamente y estarás listo para pasar a la fase de recortar la línea.


Cuando tu bucle while termina, tu variable estática (stash) contiene un bloque de texto que tiene, como mínimo, una línea completa. Imaginemos que el contenido que hay dentro del stash en ese instante es este:
"Hola Pepe\n¿Cómo estás?\n"
Tu objetivo es devolverle al usuario solo la primera línea ("Hola Pepe\n"), pero no puedes perder el resto ("¿Cómo estás?\n"), porque lo necesitarás en la siguiente llamada a get_next_line. Como una función en C solo puede devolver una cosa, dividimos el trabajo en dos ayudantes: extract_line y clean_stash.

1. ¿Qué hace extract_line? (Preparar el vaso para el usuario)

Esta función tiene una única misión: mirar lo que hay en el stash, localizar dónde está el primer \n y hacer una copia exacta desde el principio hasta ese \n incluido.
La lógica paso a paso (sin código) sería:
1. Medir hasta el \n: Creas un contador y recorres el stash carácter por carácter buscando el \n. Paras en cuanto lo encuentres (o si se acaba la cadena).
2. Pedir memoria: Haces un malloc con el tamaño exacto de lo que has contado. Le sumas + 1 para el terminador nulo \0 (y si había un \n, asegúrate de contar espacio para él también).
3. Copiar: Haces un bucle que copia los caracteres del stash a tu nueva variable local (por ejemplo, line) uno a uno.
4. Cerrar la string: Le pones el \0 al final de la copia.
5. Devolver: Devuelves line.
Resultado de este paso: Ya tienes guardado en una variable limpia el texto "Hola Pepe\n". El stash original sigue intacto de momento.

2. ¿Qué hace clean_stash? (Limpiar la mesa y guardar el sobrante)

Ahora que ya tienes la línea que vas a enviar al usuario guardada a buen recaudo, necesitas actualizar tu variable estática (stash). Si la dejas como está, la próxima vez volverás a leer "Hola Pepe\n". Tienes que "borrar" lo que ya has servido y quedarte solo con el sobrante.
La lógica paso a paso sería:
1. Buscar el punto de corte: Recorres el stash buscando el primer \n. El sobrante empieza justo en el carácter siguiente al \n.
2. Controlar si no sobra nada: Si resulta que después del \n no hay nada más (llegaste al \0 del final del archivo), significa que no hay sobrante. En ese caso, liberas el stash con un free, lo pones a NULL y terminas.
3. Pedir memoria para el sobrante: Si sí hay texto después del \n (en nuestro ejemplo, "¿Cómo estás?\n"), mides cuántos caracteres quedan hasta el final y haces un malloc con ese tamaño + 1.
4. Copiar el sobrante: Copias los caracteres de la "segunda mitad" del stash a una nueva cadena temporal. Le pones su \0 al final.
5. Liberar el pasado: Haces un free al stash antiguo (el que contenía todo el bloque largo). Esto es vital para evitar los memory leaks.
6. Devolver: Devuelves el puntero de la nueva cadena con el sobrante.
Al hacer en tu función principal stash = clean_stash(stash);, estás sustituyendo el bloque viejo por el sobrante limpio. En nuestro ejemplo, stash pasará a valer únicamente "¿Cómo estás?\n".
Al separar estas dos tareas en funciones independientes que solo procesan texto y hacen sus propios malloc, tu función principal se queda limpísima y libre de la restricción de las 25 líneas.
¿Cuál de las dos te apetece que analicemos primero a nivel de algoritmos y bucles internos para que puedas estructurarlas?
• La lógica de extract_line (recorrer y copiar hasta el \n).
• La lógica de clean_stash (saltar el \n, medir el resto, copiar y liberar la memoria vieja).