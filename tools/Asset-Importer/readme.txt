********************************************
* Tool de importaci�n de mesh y materiales *
********************************************

Permite BUSCAR y MAPEAR nombres de edificios y ficheros para importarlos en las rutas correctas del proyecto de Gale�n.
Los ficheros nunca se pisan. Si ya existen y su contenido va a ser modificado, siempre se saca un backup previo.

- Config de mapeo de nombres de edificios en: 
resources\buildingAlias.properties

- Config de tareas a ejecutar en:
resources\task.properties

- Report del import en:
out\report_<timestamp>.txt

Lanzamiento sin par�metros del programa para ver sintaxis:
java -cp target\Asset-Importer-1.0.0-jar-with-dependencies.jar trueinferno.Main

Ejemplo de lanzamiento con par�metros (desde la propia carpeta del Drive a la carpeta del proyecto):
java -cp target\Asset-Importer-1.0.0-jar-with-dependencies.jar trueinferno.Main "D:\Documentos\Proyectos\TrueInferno\galeon" "D:\GoogleDrive\True Inferno Assets\Arte"