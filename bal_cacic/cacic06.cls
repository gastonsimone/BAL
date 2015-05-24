%% Filename: cacic06.cls
%%
%% Author:  CACIC06
%% Este archivo se realiza modificando algunos fragmentos de
%% la clase standard "article". 
%%
%% Requiere disponer en su maquina : LaTeX2e y la clase estandar article.
%% Si Ud. tiene algun tipo de problemas o encuentra errores en este archivo, por favor conctactarse con:
%% E-mail: cacic2006@unsl.edu.ar
%% Http: www.cacic2006.unsl.edu.ar
%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% INSTRUCCIONES PARA EL USO DE ESTE ARCHIVO DE ESTILO
%%
%% Las siguientes lines deberian ser ubicadas al comienzo del documento
%%
%%   \documentclass{cacic06}
%%   \usepackage{times}
%%   \begin{document}
%%
%% La informacion del Titulo y Autores deberian ser escritas de la siguiente manera:
%%
%%   \title{Titulo del Paper - maximo 10 palabras}
%%   \author{ 
%%      \textbf{First Name Middle Initial Last Name}
%%      \thanks{Group supported by xxxx)} \\
%%       University Department, University Name \\
%%       City, State ZIP/Zone, Country\\
%%       e-mail address
%%    \and 
%%       \textbf{First Name Middle Initial Last Name }\\
%%       Group, Company, Address,\\
%%       City, State ZIP/Zone, Country\\
%%       e-mail address
%%   }
%%  \maketitle
%%  \thispagestyle{empty}
%%
%% El Abstract y Palabras Claves
%%
%%   \begin{abstract}
%%         El contenido de su abstract
%%   \begin{keywords}
 %%       Keywords: Keyword1, Keyword2, Keyword3, Keyword4, Keyword5.
%%   \end{keywords}
%%   \end{abstract}
%%
%% Secciones
%%
%%    \section{ }
%%
%% Referencias
%%
%%   \bibliography{mibiblio}
%%
%%   \end{document}
%%
%%
%% Las instrucciones de formateo puestas en la web han sido preparadas usando este 
%% archivo de estilos y Ud. queda habilitado para utilizar este archivo como ejemplo.
%%
%%


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%   Modificaciones a la Clase
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{cacic06}[2006/05/21 Desarrollado para el CACIC 2006]

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Opciones de la clase
% resumen : propia de esta clase
% leqno, fleqn, openbib, final, draft : como en article
%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% booleana resumen: empleada para realizar el newpage de la 
% primer carilla
%
\newif\if@resumen
\@resumenfalse
\DeclareOption{resumen}{%
      \@resumentrue%
   }

\DeclareOption{leqno}{%
      \PassOptionsToClass{\CurrentOption}{article}%
   }
\DeclareOption{fleqn}{%
      \PassOptionsToClass{\CurrentOption}{article}%
   }
\DeclareOption{openbib}{%
      \PassOptionsToClass{\CurrentOption}{article}%
   }
\DeclareOption{final}{%
      \PassOptionsToClass{\CurrentOption}{article}%
   }
\DeclareOption{draft}{%
      \PassOptionsToClass{\CurrentOption}{article}%
   }
\DeclareOption*{%
   \PackageWarning{clei}{Opci\'on desconocida `\CurrentOption'}%
}
\ProcessOptions\relax

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% REQUERIMIENTO 2 : A4
% REQUERIMIENTO 3 : 12pt
% REQUERIMIENTO 3.3 : onecolumn
\LoadClass[12pt,a4paper,oneside,notitlepage,onecolumn]{article}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% REQUERIMIENTO 2 : margenes
% para impresora sin duplex
%
\setlength{\evensidemargin}{-0.54cm}     %margen izquierdo de 2cm
\setlength{\oddsidemargin}{-0.54cm}
\setlength{\textwidth}{17cm}        %  margen derecho  2cm
\setlength\@tempdima{0.96cm}     % margen superior de 3.5 cm
\addtolength\@tempdima{-\topmargin}
\addtolength\@tempdima{-\headheight}
\addtolength\@tempdima{-\headsep}
\addtolength\topmargin{\@tempdima}
\setlength{\textheight}{24.2cm}   % margen inferior 2cm 


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% REQUERIMIENTO : acerca del titulo 
%
\renewcommand\maketitle{\par
  \begingroup
    \renewcommand\thefootnote{\@fnsymbol\c@footnote}%
    \def\@makefnmark{\rlap{\@textsuperscript{\normalfont\@thefnmark}}}%
    \long\def\@makefntext##1{\parindent 1em\noindent
            \hb@xt@1.8em{%
                \hss\@textsuperscript{\normalfont\@thefnmark}}##1}%
      \newpage
      \global\@topnum\z@   % Prevents figures from going at top of page.
      \@maketitle
    \thispagestyle{empty}\@thanks % REQUERIMIENTO 2: numeracion
  \endgroup
  \setcounter{footnote}{0}%
  \global\let\thanks\relax
  \global\let\maketitle\relax
  \global\let\@maketitle\relax
  \global\let\@thanks\@empty
  \global\let\@author\@empty
  \global\let\@date\@empty
  \global\let\@title\@empty
  \global\let\title\relax
  \global\let\author\relax
  \global\let\date\relax
%  \global\let\and\relax
}
\def\@maketitle{%
  \newpage
  \null
  \vskip 2em%
  \begin{center}%
  \let \footnote \thanks
    % REQUERIMIENTO 3.1 : fuente del titulo
    {\bfseries \fontsize{16}{20} \selectfont \@title \par}%
    \vskip 1.5em%
    { % REQUERIMIENTO 3.1 : tamanio del autor
    \lineskip .5em%
      \begin{tabular}[t]{c}%
        \@author
      \end{tabular}\par}%
    \vskip 1em%
    {\large \@date}%
  \end{center}%
  \par
  \vskip 1.5em}


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% REQUERIMIENTO caratula
%    sin fecha y \and modificado
\date{}
\renewcommand{\and}{ \\[1pc] and \\[1pc]}%
%  Sin Numeracion de Paginas
\pagestyle{empty}


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% REQUERIMIENTO 3.2 abstract y keywords
%  
\renewenvironment{abstract}{%
	\small    %% Pone la letra en tamaño 10pt, para default 12
	\begin{center}%
          {\bfseries Abstract \vspace{-.5em}\vspace{\z@}}%
        \end{center}%
      }%
      {}
\newenvironment{resumen}{%
	\small
        \begin{center}%
          {\bfseries Resumen\vspace{-.5em}\vspace{\z@}}%
        \end{center}%
        }
      {}
\if@resumen
   \newenvironment{keywords}{%
      \par \vspace{1pc} \noindent \textbf{Keywords: }%
      }%
      {}
\else
   \newenvironment{keywords}{%
      \par \vspace{1pc} \noindent \textbf{Keywords: }%
      }%
      { %\vspace{2pc}
       %\newpage
      }
\fi
\newenvironment{palabrasclaves}{%
   \par \vspace{1pc} \noindent \textbf{Palabras claves: }%
   }%
   {\newpage}


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% REQUERIMIENTO 3.4 bibliografia
%  
\bibliographystyle{plain}


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% REQUERIMIENTO 4 secciones
%  
\renewcommand\section{\@startsection {section}{1}{\z@}%
                                   {-3.5ex \@plus -1ex \@minus -.2ex}%
                                   {2.3ex \@plus.2ex}%
                                   {\normalfont\large\bfseries\MakeUppercase}}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% REQUERIMIENTO 4.1 subsecciones
%  
\renewcommand\subsection{\@startsection{subsection}{2}{\z@}%
                                     {-3.25ex\@plus -1ex \@minus -.2ex}%
                                     {1.5ex \@plus .2ex}%
                                     {\normalfont \bfseries}}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% REQUERIMIENTO 4.1.1 subsubsecciones
%  
\renewcommand\subsubsection{\@startsection{subsubsection}{3}{\z@}%
                                     {-3.25ex\@plus -1ex \@minus -.2ex}%
                                     {1.5ex \@plus .2ex}%
                                     {\normalfont\itshape}}


