#ifndef _WINDOWS_H_
#define _WINDOWS_H_


    // Un elemento de pantalla (sE) tiene ubicacion y tamaño

        #define SCREEN_ELEM  \
            uBYTE  cols;     \
            uBYTE  rows;     \
            BYTE   x = 0;    \
            BYTE   y = 0;    \

    // Una pestaña (tab) es un sE que contiene datos (buff)

        struct Tab { SCREEN_ELEM
            char         * buff;    
        };

    // Una ventana (w) es un sE que Debería contener tabs u otras w
        struct Window { SCREEN_ELEM
            struct Node       * sE;
        };


    #define _MOVE_INDEX(n,i,L)   if ( i + n <= L  && i + n >= -L ) i += n;
    #define _TABS(w)            (w)->sE.tabs
    #define _TAB(i,w)           ( (Tab*) ( getFromLast(i, _TABS(w))->e ) )
    #define _BUFF(tab)          (char *)( (tab)->e.buff + (tab)->y * (tab)->cols + (tab)->x )

    #define W_POS(w)            (w)->x ,  (w)->y
    #define W_SIZE(w)           (w)->cols ,  (w)->rows
    #define V_MOVE(n,e)         _MOVE_INDEX( n, (e)->y, (e)->rows )
    #define H_MOVE(n,e)         _MOVE_INDEX( n, (e)->x, (e)->cols )
   
    #define BUFF(i,w)           _BUFF( _TAB(i,w) )
    #define V_SCROLL(n,i,w)     _MOVE_INDEX( n, _TAB(i,w)->y, (w)->rows )
    #define H_SCROLL(n,i,w)     _MOVE_INDEX( n, _TAB(i,w)->x, (w)->cols )
     

    /* 
     * Ejemplo de uso:
     *
     *  char buff = "MENU :\n"
     *              "  1)Comenzar\n"
     *              "  2)Config." ;
     *
     *  Window w = newEmptyWindow(0,0,2,16);    // Ventana: 2 renglones de 16 caracteres c/u
     *  loadTab(w,&buff);                       // Contenido: 3 renglones
     */
    
    // Una ventana sin pestañas
    struct ScreenElem * newEmptyWindow(BYTE left, BYTE top, uBYTE cols, uBYTE rows);
     
    BOOL loadTab(Window *w, uBYTE cols, uBYTE rows, char *buff);

    struct ScreenElem * loadWindow(BYTE left, BYTE top, uBYTE cols, uBYTE rows, char *buff);

    struct ScreenElem * newWindow(BYTE left, BYTE top, uBYTE cols, uBYTE rows);
    BOOL addTab(Window *w, uBYTE cols, uBYTE rows);

    void cleanTab();

    void closeWindow();
    void closeTab();

#endif