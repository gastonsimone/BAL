#ifndef __BAL_H__
#define __BAL_H__

#include <stdio.h>
#include "sparse/sp_coord.h"
#include "sparse/sp_packcol.h"
#include "sparse/sp_cds.h"

/** \mainpage Biblioteca the Álgebra Lineal (BAL)
 *
 * \section introsec Introducción
 *
 * BAL es una biblioteca que contiene un conjunto de utilidades e implementaciones de algoritmos esenciales
 * para el estudio del álgebra lineal numérica.
 * Está centrada en técnicas utilizadas para matrices dispersas, pero no necesariamente se limita a este campo.
 *
 * El objetivo principal de esta biblioteca es didáctico. Pretende servir como ejemplo de implementación
 * acompañada de documentación de estructuras de datos y algoritmos para matrices dispersas.
 *
 * Es de destacar que esta documentación es totalmente generada a partir de los archivos fuentes de
 * BAL (incluso esto que estás leyendo).
 *
 * \section historysec Historia de BAL
 * 
 * <UL>
 *   <LI><B>Versión:</B> 1.0.0</LI>
 *   <LI><B>Cuándo:</B> Curso ALN 2008</LI>
 *   <LI><B>Quién:</B> Gastón Simone (<A href="mailto:gaston.simone@gmail.com">gaston.simone@gmail.com</A>) con la colaboración de Pablo Ezzatti (<A href="mailto:pezzatti@fing.edu.uy">pezzatti@fing.edu.uy</A>)</LI>
 *   <LI><B>Descripción:</B> Implementación inicial</LI>
 *   <LI><B>Notas:</B> \ref notasver1_0_0 "Ver notas"</LI>
 *   <LI><B>Detalle:</B></LI>
 *     <UL>
 *       <LI>Cargas de datos desde archivo</LI>
 *         <UL>
 *           <LI>Parser de definición de matrices en formato matlab leídas desde archivo (bal_cargar_matriz())</LI>
 *           <LI>Carga de matrices en formato simple por coordenadas desde archivo (bal_load_coord())</LI>
 *         </UL>
 *       <LI>Distintos formatos de matrices dispersas:</LI>
 *         <UL>
 *           <LI>Simple por coordenadas (sp_coord)</LI>
 *           <LI>Empaquetado por columnas (sp_packcol)</LI>
 *           <LI>CDS o comprimido por diagonales (sp_cds)</LI>
 *         </UL>
 *       <LI>Funciones para imprimir estructuras de datos:</LI>
 *         <UL>
 *           <LI>Matriz completa (bal_imprimir_matriz())</LI>
 *           <LI>Detalle para formato simple por coordenadas (bal_imprimir_coord())</LI>
 *           <LI>Detalle para formato empaquetado por columna (bal_imprimir_packcol())</LI>
 *           <LI>Detalle para formato CDS (bal_imprimir_cds())</LI>
 *           <LI>De simple por coordenadas a formato texto específico (bal_save_coord())</LI>
 *           <LI>De empaquetado por columna a formato matlab (bal_save_packcol(), bal_save_packcol_symmetric())</LI>
 *           <LI>De CDS a formato matlab (bal_save_cds())</LI>
 *         </UL>
 *       <LI>Conversiones entre formatos:</LI>
 *         <UL>
 *           <LI>De matriz completo a simple por coordenadas (bal_mat2coord())</LI>
 *           <LI>De simple por coordenadas a empaquetado por columnas (bal_coord2packcol())</LI>
 *           <LI>De simple por coordenadas a empaquetado por columnas específico para matrices simétricas (bal_coord2packcol_symmetric())</LI>
 *           <LI>De simple por coordenadas a CDS (bal_coord2cds())</LI>
 *         </UL>
 *       <LI>Funciones de soporte:</LI>
 *         <UL>
 *           <LI>Funciones de liberación de memoria reservada por estructuras de datos (bal_free_coord(), bal_free_packcol(), bal_free_cds())</LI>
 *         </UL>
 *       <LI>Operaciones básicas:</LI>
 *         <UL>
 *           <LI>Multiplicar una matriz empaquetada por columna por un vector (bal_mult_vec_packcol())</LI>
 *           <LI>Multiplicar una matriz simétrica empaquetada por columna por un vector (bal_mult_vec_packcol_symmetric())</LI>
 *           <LI>Multiplicar una matriz CDS por un vector (bal_mult_vec_cds())</LI>
 *           <LI>Multiplicar dos matrices CDS (bal_mult_mat_cds())</LI>
 *           <LI>Permutar las columnas de una matriz empaquetada por columna (bal_permutar_packcol())</LI>
 *         </UL>
 *       <LI>Factorización de Cholesky (bal_cholesky_solver())</LI>
 *         <UL>
 *           <LI>Funcionalidad para recorrer eficientemente por filas una matriz dispersa empaquetada por columnas (bal_row_traversal_packcol())</LI>
 *           <LI>Funcionalidad para calcular el árbol de eliminación de una matriz empaquetada por columna (bal_elimination_tree())</LI>
 *           <LI>Factorización simbólica para matrices simétricas empaquetadas por columna (bal_symbolic_factorization())</LI>
 *           <LI>Factorización numérica (algoritmo de Cholesky) para matrices simétricas empaquetadas por columna (bal_numerical_factorization())</LI>
 *           <LI>Resolución de sistemas con matrices triangulares empaquetadas por columna (bal_cholesky_Lsolver(), bal_cholesky_LTsolver())</LI>
 *         </UL>
 *       <LI>Programas de prueba de todas las funcionalidades implementadas (ver la sección de \ref testsec "Pruebas")</LI>
 *     </UL>
 * </UL>
 *
 * \todo
 *
 * A continuación se enumeran algunas de las características que por distintas razones no fueron implementadas en BAL 1.0.0,
 * pero que destacamos como importantes.
 * Algunas de las tareas ya se están abordando mientras que otras se preve atacarlas en un futuro cercano.
 *
 *       Soporte para más estructuras de matrices dispersas.<BR>
 *       Más conversiones entre estructuras de matrices dispersas.<BR>
 *       Extender el parser para que cargue una matriz desde archivo directamente en un formato disperso.<BR>
 *       Algoritmos de reordenamiento (ver reordenamiento.c).<BR>
 *       Más algoritmos de factorización simbólica.<BR>
 *       Más algoritmos de factorización numérica (LU por ejemplo).<BR>
 *       Métodos iterativos (Jacobi, Gauss-Seidel, Gradiente conjugado, etc).<BR>
 *       Algoritmos de creación de precondicionadores (ILU por ejemplo).<BR>
 *       Agregar salidas detalladas para cada uno de los algoritmos, en base a un parámetro, mediante el que se pueda controlar
 *       la información a mostrar. Por ejemplo: memoria utilizada, tiempos de ejecución, cantidad de operaciones.<BR>
 *       Extender las funciones que corresponda (por ejemplo, cholesky_solver()) para que reciban un vector de parámetros, en el
 *       cual se indique la estrategia de ordenamiento, el método de factorización simbólica, etc.<BR>
 *       Agregar soporte para paralelismo.<BR>
 *       Agregar soporte para el uso de bibliotecas BLAS.<BR>
 *       Agregar algoritmos que utilicen técnicas por bloques.<BR>
 *       Rutinas de refinamiento iterativo.<BR>
 *       Vectores y valores propios.<BR>
 *       Escalado.<BR>
 *       Agregar la posibilidad de compilar/instalar sin el parser (para sistemas que no cumplan con todas las dependencias necesarias).<BR>
 *
 * \section howtosec Cómo usar BAL
 *
 * En esta sección se describe información de utilidad para poder trabajar con BAL, ya sea extendiendo BAL o simplemente utilizándola.
 *
 * \subsection reqsec Requisitos para compilar BAL
 *
 * La biblioteca fue desarrollada y probada bajo la plataforma GNU/Linux (concretamente en Ubuntu 7.10). Sin embargo no hay razón para que BAL no
 * funcione en otros sistemas, incluso con Windows y <A href="http://es.wikipedia.org/wiki/Cygwin">Cygwin</A>.
 *
 * Todos los requisitos para ejecutar BAL deberían estar disponibles en las distribuciones más utilizadas de Linux (no quizás en la instalación por defecto, pero sí como paquetes fácilmente instalables), sin embargo se agregaron enlaces a la lista para que sea más fácil encontrar las dependencias en caso de ser necesario.
 *
 * BAL depende de los siguientes componentes para compilar correctamente:
 * <UL>
 *   <LI><A href="http://es.wikipedia.org/wiki/GCC">GNU Compiler Collection (GCC)</A>. Concretamente el compilador de C (BAL fue probado con la versión 4.1.3).</LI>
 *   <LI><A href="http://www.gnu.org/software/make/">GNU Make</A> (BAL fue probado con la versión 3.81)</LI>
 *   <LI><A href="http://es.wikipedia.org/wiki/GLib">GLib</A> (BAL fue probado con la versión 2.0)</LI>
 *   <LI><A href="http://es.wikipedia.org/wiki/GNU_Bison">GNU Bison</A> (BAL fue probado con la versión 2.3)</LI>
 *   <LI><A href="http://en.wikipedia.org/wiki/Flex_lexical_analyser">Flex</A> (BAL fue probado con la versión 2.5.33)</LI>
 * </UL>
 *
 * Aunque no es estrictamente necesario, puede ser útil contar con los siguientes componentes adicionales:
 * <UL>
 *   <LI><A href="http://es.wikipedia.org/wiki/Doxygen">Doxygen</A> (necesario para generar esta documentación, BAL fue probado con la versión 1.5.3)</LI>
 *   <LI>Una distribución del sistema <A href="http://es.wikipedia.org/wiki/LaTeX">LaTeX</A> (necesario para generar la documentación, tanto en PDF como en HTML, BAL fue probado con <A href="http://en.wikipedia.org/wiki/TeX_Live">TeX Live</A> 2007)</LI>
 *   <LI><A href="http://es.wikipedia.org/wiki/GNU_Debugger">GNU Debugger (gdb)</A></LI>
 *   <LI>Alguna interfaz gráfica para gdb, como <A href="http://es.wikipedia.org/wiki/Anjuta">Anjuta</A>, <A href="http://sourceware.org/insight/">insight</A>, <A href="http://es.wikipedia.org/wiki/Data_Display_Debugger">Data Display Debugger (ddd)</A> o <A href="http://home.gna.org/nemiver/">Nemiver</A></LI>
 *   <LI>Un generador de tablas de referencias al estilo <A href="http://en.wikipedia.org/wiki/Ctags">CTags</A> (para el desarrollo de BAL se utilizó <A href="http://ctags.sourceforge.net/">Exuberant CTags</A>)</LI>
 * </UL>
 *
 * \subsection compilesec Cómo compilar e instalar BAL
 *
 * BAL incluye un conjunto de <TT>makefiles</TT> que automatizan el trabajo de compilación e instalación.
 * A continuación se listan las distintas llamadas que se le pueden realizar al <TT>makefile</TT> de BAL:
 * <UL>
 *  <LI><TT>make</TT>: Este modo compila todo lo necesario para generar la biblioteca BAL. El resultado principal es el archivo <TT>libbal.a</TT></LI>
 *  <LI><TT>make clean</TT>: Borra todos los archivos intermedios generados, necesarios para construir BAL.</LI>
 *  <LI><TT>make purge</TT>: Igual que <TT>make clean</TT>, pero también borra la biblioteca BAL generada.</LI>
 *  <LI><TT>make tags</TT>: Genera el archivo de etiquetas mediante una herramienta ctags.</LI>
 *  <LI><TT>make doc</TT>: Genera esta documentación a partir de los archivos fuente (se recomienda ejecutarlo dos veces). <B>NOTA</B>: Se recomienda hacer un <TT>make clean</TT> antes de generar la documentación. Si se genera la documentación con los fuentes generados para el parser, la misma contendrá una gran cantidad de referencia a código autogenerado (muy feo) y no propiamente documentado que hacen que la documentación no se vea bien.</LI>
 *  <LI><TT>make cleandoc</TT>: Borra la documentación generada.</LI>
 *  <LI><TT>make install</TT></LI>: Instala BAL como una biblioteca más del sistema (para sistemas UNIX). Este comando debe ser ejecutado con permisos de administrador (root).
 *  <LI><TT>make uninstall</TT></LI>: Deshace lo hecho con el comando <TT>make install</TT>. También debe ser ejecutado como el usuario root.
 * </UL>
 *
 * Es importante aclarar que la compilación de BAL no produce ningún código directamente ejecutable. Solo produce un archivo (<TT>libbal.a</TT>) ya compilado, pronto para ser enlazado (\em linkeado) con la aplicación que necesite utilizar las prestaciones de BAL. En la siguiente sección se da detalle de cómo utilizar BAL desde otros programas (ver \ref usesec "Cómo usar BAL desde otro programa").
 *
 * \subsubsection installsec Secuencia de instalación
 *
 * Si se tienen todas las dependencias necesarias y todo va bien, la siguiente secuencia de comandos compila, genera la documentación e instala BAL en el sistema:
 *
 * <CODE>
 * make doc<BR>
 * make<BR>
 * sudo make install
 * </CODE>
 *
 * El comando \c sudo provoca que el comando a continuación se ejecute con permisos de root. Puede llegar a pedir una contraseña.
 *
 * \subsubsection compilemodesec Modos de compilación
 *
 * Los archivos <TT>makefile</TT> de BAL (el propio Makefile y los archivos *.mak) definen una variable llamada <TT>CFLAGS</TT>, la cual indica algunos argumentos extra a utilizar a la hora de invocar al compilador de C. Los archivos contienen dos definiciones para esta variable, una específica para depuración de BAL y otra para generar código optimizado. Revise los archivos <TT>makefile</TT> y utilice la definición que más se ajuste a sus necesidades.
 *
 * \subsection usesec Cómo usar BAL desde otro programa
 *
 * En esta sección se muestra cómo escribir código que utilice las implementaciones de BAL y luego cómo compilarlo.
 *
 * \subsubsection refusesec Cómo referenciar a BAL desde código externo
 *
 * Escribir código que utilice BAL es muy sencillo (la parte, un poco, más compleja es a la hora de compilar y es detalla en la siguiente sección).
 * Simplemente se debe agregar la referencia a BAL
 *
 * <CODE>
 * \#include <bal.h>
 * </CODE>
 *
 * y luego utilizar las funciones descritas en este documento.
 * Es importante recordar que el punto de entrada a BAL desde un programa externo es siempre y únicamente el archivo de cabecera
 * <TT>bal.h</TT>. Toda la funcionalidad de BAL es expuesta mediante este archivo.
 * Por lo tanto, es suficiente con incluir este archivo en el programa "cliente".
 *
 * \subsubsection compileusesec Cómo compilar un programa que usa BAL
 *
 * La mejor forma de ver cómo utilizar BAL es viendo cómo se hizo un programa que la usa. Junto con BAL se distribuye un juego de programas de prueba que hacen justamente esto. Utilizan BAL para probarla. De todos modos, a continuación se describe cómo sería una llamada al compilador de C que enlace BAL con el programa cliente.
 *
 * Supongamos, para facilitar el ejemplo que nuestro programa cliente consta de un solo archivo <TT>miprog.c</TT> y que este archivo lo precompilamos de la manera clásica produciendo el archivo <TT>miprog.o</TT>. Esto lo obtenemos con un comando similar al siguiente:
 *
 * <CODE>
 * gcc -c miprog.c -o miprog.o
 * </CODE>
 *
 * Ahora lo que resta es la etapa de enlazado. Supongamos que tenemos BAL
 * (particularmente, el archivo <TT>libbal.a</TT> y los archivos de cabecera de BAL) compilada y pronta en el directorio <TT>/bal</TT>
 * (aunque podría ser cualquier directorio), pero no instalamos BAL con el comando <TT>make install</TT>.
 * El comando para enlazar es el siguiente:
 *
 * <CODE>
 * gcc -I/bal miprog.o -o miprog -L/bal -lbal `pkg-config --cflags --libs glib-2.0` -lfl -lm
 * </CODE>
 *
 * Esto generará un programa ejecutable llamado <TT>miprog</TT>.
 * Expliquemos ahora la composición de esa llamada a <TT>gcc</TT>:
 * <UL>
 *  <LI>El argumento <TT>-I/bal</TT> le indica al compilador que debe agregar el directorio <TT>/bal</TT> a su lista de directorios de búsqueda para la resolución de las instrucciones de precompilador tipo <TT>\#include</TT>.</LI>
 *  <LI>El argumento <TT>-L/bal</TT> le indica al compilador que debe agregar el directorio <TT>/bal</TT> a su lista de directorios de búsqueda para la resolución de los comandos <TT>-l</TT>.</LI>
 *  <LI>El argumento <TT>-lbal</TT> le indica al compilador que debe enlazar la biblioteca <TT>libbal.a</TT> al programa resultante.</LI>
 *  <LI>La ejecución embebida <TT>`pkg-config --cflags --libs glib-2.0`</TT> genera las banderas necesarias para enlazar <TT>glib 2.0</TT>, necesario por BAL. Se puede probar ejecutar solo esto para ver el resultado que produce. De no contar con la herramienta <TT>pkg-config</TT>, se pueden agregar las referencias manualmente. Las mismas serían similares a: <TT>-I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include  -lglib-2.0</TT></LI>
 *  <LI>Los argumentos <TT>-lfl</TT> y <TT>-lm</TT> enlazan las bibliotecas <TT>libfl.a</TT> y <TT>libm.a</TT> respectivamente, la primera necesaria por el uso de <TT>glib</TT> y la segunda necesaria por BAL.
 * </UL>
 *
 * Si instalamos BAL con el comando <TT>make install</TT>, podemos referenciar a BAL en el código cliente con la instrucción
 *
 * <CODE>
 * \#include <BAL/bal.h>
 * </CODE>
 *
 * y el comando de compilación sería el siguiente:
 * 
 * <CODE>
 * gcc miprog.o -o miprog -lbal `pkg-config --libs glib-2.0` -lfl -lm
 * </CODE>
 *
 * \section testsec El juego de pruebas de BAL
 *
 * BAL contiene un conjunto de programas que sirven, no solo para verificar la correctitud de BAL, sino también como
 * ejemplo de su uso.
 *
 * Estos tests se encuentran en el directorio \c bal_test. Cada prueba es un programa C diferente y básicamente cada
 * prueba se limita a probar una funcionalidad particular de BAL. El nombre del archivo <TT>.c</TT> da idea de lo que
 * se pretende probar.
 *
 * El juego de pruebas tiene su propio \c makefile para la fácil compilación. Pero más aun, el directorio de pruebas
 * contiene un script llamado \c run_tests.sh para la ejecución fácil de las pruebas. Este script, primero ejecuta \c make para cerciorar que
 * los tests han sido compilados. Luego busca aquellos tests que fueron compilados correctamente y los ejecuta.
 * Para cada test ejecutado, redirecciona su salida estándar a un archivo de extensión <TT>.out</TT> y su salida de errores
 * a una archivo de extensión <TT>.error</TT>. El prefijo de ambos archivos es el nombre del ejecutable del test.
 *
 * <B>NOTA</B>: Para que este mecanismo funcione es importante que todos los ejecutables de las pruebas tengan sufijo <TT>_test</TT>.
 *
 * Al finalizar, el script muestra en pantalla una lista de aquellos tests que generaron algún tipo de salida en la
 * salida de errores (<em>standard error</em>). De este modo es fácil localizar cuáles tests fallaron.
 *
 * \section refsec Referencias
 *
 * La siguiente es una lista de documentos que fueron utilizados para la implementación de BAL:
 * <UL>
 *   <LI>G. W. Stewart. <I>Building an Old-Fashioned Sparse Solver</I>. Agosto 2003.<BR>
 *       Department of Computer Science and Institute for Advanced Computer Studies, University of Maryland<BR>
 *       <A href="http://hdl.handle.net/1903/1312">http://hdl.handle.net/1903/1312</A></LI>
 * </UL>
 *
 * \section licsec Licencia
 *
 * <UL>
 *   <LI>Está permitida toda copia (total o parcial, digital o impresa) y distribución de las copias, tanto de esta documentación como del código fuente que forma parte de BAL. Incluso está permitido modificar este trabajo.</LI>
 *   <LI>Está permitida la difusión de este trabajo, en cualquiera de las condiciones amparadas por el enunciado anterior y cualquier otra que se le ocurra al difusor.</LI>
 *   <LI>Está permitido cualquier tipo de uso de este material, ya sea personal, comercial o cualquier otro.</LI>
 *   <LI>Está permitido mentir acerca del origen de este trabajo, de todos modos nadie se va a enterar. Pero esta actitud pesará eternamente en la conciencia de la persona que lo haga y en la conciencia de sus hijos, y los hijos de sus hijos, etc.</LI>
 *   <LI><B>Este trabajo (software y documentación) se entrega "TAL CUAL" y se renuncia a toda responsabilidad por las garantías, implícitas o explícitas, incluidas, sin limitación. UTILÍCELO BAJO SU PROPIA RESPONSABILIDAD.</B></LI>
 * </UL>
 */

/** \page notasver Notas de las versiones de BAL
 *
 * \section notasver1_0_0 Notas a la versión 1.0.0 de BAL
 *
 * Al ser esta la primer versión de BAL, hubo un trabajo fuerte en armar la estructura básica de la biblioteca.
 * Para ello se tuvo que estudiar la forma de armar bibliotecas en ambientes UNIX.
 *
 * Por otra parte, se hizo hincapié en la documentación. Por ello se utilizó la herramienta Doxygen para generar documentación a partir
 * del código. Esto facilitó mucho la tarea. Espero que se encuentre útil la documentación generada y que las próximas versiones también
 * utilicen este mecanismo para documentar sus extensiones.
 *
 * Al partir de cero, fue necesario implementar algunas funcionalidades básicas para comenzar con la biblioteca,
 * por ejemplo el parser de matrices en formato matlab, las funciones de impresión, etc.
 *
 * Por otra parte, junto con BAL se entregan en esta versión un conjunto de programas que sirven no solo como pruebas de correctitud
 * de las funcionalidades de BAL, sino también como ejemplos de cómo utilizar BAL y las salidas que produce. Éstos se pueden
 * encontrar en el directorio \c bal_test, el cual también viene con un \c Makefile para la fácil compilación.
 *
 * Sin embargo, la implementación más importante de esta versión es sin duda el algoritmo de Cholesky. Por tanto podemos decir
 * que la función más importante de esta versión es cholesky_solver(), la cual engloba los algoritmos más importantes y complejos
 * implementados.
 *
 * La gran mayoría de los algoritmos implementados en esta versión (sobre todo los más complejos) están basados en las descripciones del paper
 * <em>Building an Old-Fashioned Sparse Solver</em>, por G. W. Stewart (ver las \ref refsec "referencias").
 * Aunque el solver es <em>old-fashioned</em> esto no quiere decir que sea un mal solver. Citando al propio Stewart,
 * <em>No es un juguete. Cerca de 1975, grandes investigadores estaban trabajando duro para perfeccionar un solver como el nuestro.</em>
 * Esto es importante señalarlo. Porque si bien no es una implementación con lo último del estado del arte, es una excelente muestra
 * para el aprendizaje de solvers profesionales (de hecho este solver lo fue en algún momento!).
 *
 * Personalmente recomiendo mucho la lectura del paper de Stewart. Está muy bien detallado. Sin embargo, algunos de los pseudo-códigos
 * del paper tienen errores (muy sutiles). Pero dichos errores fueron corregidos durante la implementación de BAL.
 * Por tanto se recomienda comparar el pseudo-código de Stewart con la implementación en BAL para cada uno de los algoritmos. Creo que 
 * esto ayudará a entender mejor los algoritmos, que no son nada triviales.
 *
 * Gastón Simone - Mayo 2008
 *
 */

/** \dir ./parser
 *  \brief Directorio con implementación del parser de matrices en formato matlab.
 *
 *  Este directorio contiene todos los archivos que implementan el parser de matrices en formato matlab, así como también un archivo
 *  \c makefile para la fácil generación del parser.
 */

/** \dir ./sparse
 *  \brief Directorio con archivos de estructuras de matrices dispersas
 *
 *  Este directorio contiene las definiciones de las estructuras de datos para matrices dispersas, así como también la implementación
 *  de las funciones de conversión entre estas representaciones.
 */

/** \dir ./cholesky
 *  \brief Directorio con implementación de la factorización de Cholesky
 */

/** \file bal.h
 *  \brief Biblioteca de Algebra Lineal, archivo de cabecera
 *
 *  Este archivo define las funciones disponibles mediante BAL.
 */

int bal_cargar_matriz(const char *archivo, double ***matriz, int *n, int *m);

void bal_imprimir_matriz(FILE *fp, double **matriz, int n, int m);

sp_coord* bal_mat2coord(int n, int m, double **mat);

sp_packcol* bal_coord2packcol(sp_coord* mat);

sp_packcol* bal_coord2packcol_symmetric(sp_coord* mat);

sp_cds* bal_coord2cds(sp_coord *mat);

void bal_imprimir_coord(FILE *fp, sp_coord *mat);

void bal_imprimir_packcol(FILE *fp, sp_packcol *mat);

void bal_imprimir_cds(FILE *fp, sp_cds *mat);

void bal_mult_vec_packcol(sp_packcol *A, double *x, double *y);

void bal_mult_vec_packcol_symmetric(sp_packcol *A, double *x, double *y);

int bal_row_traversal_packcol(sp_packcol *A, int *i, int *j, int *posij);

sp_coord* bal_load_coord(FILE *fp);

void bal_save_coord(FILE *fp, sp_coord *A);

void bal_save_packcol_symmetric(FILE *fp, sp_packcol *A);

void bal_save_packcol(FILE *fp, sp_packcol *A);

void bal_save_cds(FILE *fp, sp_cds *A);

int *bal_elimination_tree(sp_packcol *A, int *nnz);

sp_packcol *bal_symbolic_factorization(sp_packcol *A);

void bal_numerical_factorization(sp_packcol *A, sp_packcol *L);

void bal_cholesky_Lsolver(sp_packcol *L, double *b);

void bal_cholesky_LTsolver(sp_packcol *L, double *b);

void bal_cholesky_solver(sp_packcol *A, double *b);

void bal_mult_vec_cds(sp_cds *A, double *x, double *y);

sp_cds* bal_mult_mat_cds(sp_cds *A, sp_cds *B);

sp_packcol* bal_permutar_packcol(unsigned int *p, sp_packcol *A);

void bal_free_coord(sp_coord *A);

void bal_free_packcol(sp_packcol *A);

void bal_free_cds(sp_cds *A);

#endif

