#include "ZLib.h"

#define ADD_TAB(tab,w)      addElem(tab, (struct Node*) _TABS(w));


struct Window * loadWindow(BYTE left, BYTE top, uBYTE cols, uBYTE rows, char *buff) { 
  ScreenElem *w = newEmptyWindow(left, top, cols, rows);
  if ( w  == NULL )
    return NULL;
  if ( loadTab(w, cols, rows, buff) )
    return w;
  return NULL;
}

struct Window * newWindow(BYTE left, BYTE top, uBYTE cols, uBYTE rows) { 
  ScreenElem *w = newEmptyWindow(left, top, cols, rows);
  if ( w == NULL )
    return NULL;
  if ( addTab(w, cols, rows) )
    return w;
  return NULL;
}

ScreenElem wM;
struct Window * newEmptyWindow(BYTE left, BYTE top, uBYTE cols, uBYTE rows) { 
  ScreenElem *w = &wM; /*(ScreenElem) reserveMem( sizeof(ScreenElem) ) */
  if ( ! w )
    return NULL;
  w->e.tabs = newList();
  w->x=left; w->y=top;
  w->cols=cols; w->rows=rows;
  return w;
}

BOOL addTab(Window *w, uBYTE cols, uBYTE rows) {
  if( ! loadTab( w, cols, rows, reserveMem(cols * rows)  ) ) // RESERVE MEMORY
    return FALSE;
  cleanTab();
  return TRUE;
}

ScreenElem tabM;
BOOL loadTab(Window *w, uBYTE cols, uBYTE rows, char *buff) {
  ScreenElem *tab = &tabM; /*(ScreenElem) reserveMem( sizeof(ScreenElem) )*/ 
  if ( ! tab )
    return FALSE; 
  tab->e.buff = buff; tab->cols=cols; tab->rows=rows;
  ADD_TAB(tab, w);
  return TRUE;
}



void cleanTab(){

}

char mem[10];
char * reserveMem(int dim) {
  dim = dim;
  return &mem[0];
}
