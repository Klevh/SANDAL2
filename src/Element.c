#include "Element.h"

#include <math.h>

/* -------------------------------------------------------
 * Other functions
 */
static void copyColor(int to[4],int from[4]){
    to[0]=from[0];
    to[1]=from[1];
    to[2]=from[2];
    to[3]=from[3];
}

static void replaceChildren(ListPtrElement l, double x, double y){
    PtrElement * p = l.first;

    while(p){
        p->element->x += x;
        p->element->y += y;
        replaceChildren(p->element->elementChildren, x, y);

        p = p->next;
    }
}

static void rotateChildren(ListPtrElement l, double a){
    PtrElement * p = l.first;
    double x, y, newX, newY, prX, prY;
    double c, s, oldA, hyp;

    if(p){
        prX = p->element->elementParent->prX * p->element->elementParent->width + p->element->elementParent->x;
        prY = p->element->elementParent->prY * p->element->elementParent->height + p->element->elementParent->y;
        
        while(p){
            rotateElement(p->element, a, prX, prY);
            p = p->next;
        }
    }
}

static void redimChildren(ListPtrElement l, double percentX, double percentY){
    PtrElement * p = l.first;
    double newW, newH;
    double newX, newY;
    double centerX, centerY;
    double x, y;

    while(p){
        newW = p->element->width * percentX;
        newH = p->element->height * percentY;

        centerX = p->element->elementParent->x;
        centerY = p->element->elementParent->y;

        x = p->element->x;
        y = p->element->y;

        newX = centerX + (x - centerX) * percentX;
        newY = centerY + (y - centerY) * percentY;
        
        replaceElement(p->element, newX, newY);
        setDimensionElement(p->element, newW, newH);

        p = p->next;
    }
}
/* ------------------------------------------------------- */



/* -------------------------------------------------------
 * Liste d'elements  avec display code commun
 */
static ListPtrElement* initListPtrElement(int plan){
    ListPtrElement *l=(ListPtrElement*)malloc(sizeof(*l));

    if(l){
        l->first=NULL;
        l->last=NULL;
        l->current=NULL;
        l->next=NULL;
        l->code=plan;
    }

    return l;
}

static void freeListPtrElement(ListPtrElement* l){
    PtrElement * ptr, * ptmp;
    
    if(l){
	ptr = l->first;
        while(ptr){
            ptmp=ptr->next;
	    free(ptr);
            ptr=ptmp;
        }
	free(l);
    }
}

static int addPtrElement(ListPtrElement* l,Element* e){
    DisplayCode *d;
    PtrElement *p;
    int error = 1;
  
    if(l && e){
        d=e->codes->first;
        while(d && d->plan > l->code){
            d=d->next;
        }
        if(d && d->plan==l->code){
            p=(PtrElement*)malloc(sizeof(*p));
            if(p){
                if(l->last){
                    l->last->next=p;
                }else{
                    l->first=p;
                }
                l->last=p;
                p->next=NULL;
		p->deleted = 0;
                p->element=e;
                error = 0;
            }
        }
    }

    return error;
}
/* ------------------------------------------------------- */



/* -------------------------------------------------------
 * Liste de liste (plan) d'element, contenant le code 
 * d'affichage
 */
static ListDCElement* initListDCElement(int displayCode){
    ListDCElement * l =(ListDCElement*)malloc(sizeof(*l));

    if(l){
        l->first=NULL;
        l->current=NULL;
        l->next=NULL;
        l->code=displayCode;
    }

    return l;
}

static void freeListDCElement(ListDCElement* l){
    ListPtrElement *lp,*lptmp;

    if(l){
        lp=l->first;
        while(lp){
            lptmp=lp->next;
            freeListPtrElement(lp);
            lp=lptmp;
        }
	free(l);
    }
}
/* ------------------------------------------------------- */



/* -------------------------------------------------------
 * Liste de liste (code d'affichage) de liste (plan) 
 * d'element
 */
ListElement* _initListElement(){
    ListElement *l=(ListElement*)malloc(sizeof(*l));
  
    if(l){
        l->first=NULL;
        l->currentDCIterator=NULL;
        l->currentPIterator=NULL;
    }
  
    return l;
}

void _freeListElement(ListElement *l){
    ListDCElement *lp,*ltmp;

    if(l){
        lp=l->first;
        while(lp){
            ltmp=lp->next;
            freeListDCElement(lp);
            lp=ltmp;
        }
        free(l);
    }
}

int addElement(Element* e){
    DisplayCode *d;
    ListDCElement ** ldc, *dctmp;
    ListPtrElement ** lp, *ptmp;
    int error = 1;
  
    if(_windows_SANDAL2 && _windows_SANDAL2->current && _windows_SANDAL2->current->liste && e && e->codes){
        error = 0;
        d=e->codes->first;
        ldc=&(_windows_SANDAL2->current->liste->first);
        while(d){
            while(*ldc && (*ldc)->code < d->code){
                ldc=&((*ldc)->next);
            }
            if(!*ldc){
                *ldc=initListDCElement(d->code);
                if(d->code == _windows_SANDAL2->current->displayCode && !(_windows_SANDAL2->current->current))
                    _windows_SANDAL2->current->current = *ldc;
                lp=&((*ldc)->first);
                *lp=initListPtrElement(d->plan);
            }else if((*ldc)->code!=d->code){
                dctmp=initListDCElement(d->code);
                if(d->code == _windows_SANDAL2->current->displayCode && !(_windows_SANDAL2->current->current))
                    _windows_SANDAL2->current->current = dctmp;
                dctmp->next=(*ldc);
                (*ldc)=dctmp;
                lp=&((*ldc)->first);
                *lp=initListPtrElement(d->plan);
            }else{
                lp=&((*ldc)->first);
                while(*lp && (*lp)->code > d->plan){
                    lp=&((*lp)->next);
                }
                if(!*lp){
                    *lp=initListPtrElement(d->plan);
                }else if((*lp)->code != d->plan){
                    ptmp=*lp;
                    *lp=initListPtrElement(d->plan);
                    (*lp)->next=ptmp;
                }
            }
            error |= addPtrElement(*lp,e);
            d=d->next;
        }
        e->parent=_windows_SANDAL2->current;
    }

    return error;
}

void _cleanElement(){
    PtrElement **e, *etmp, *prev;
    ListPtrElement **lp, *ptmp;
    ListDCElement **ldc, *dctmp;
  
    if(_windows_SANDAL2 && _windows_SANDAL2->current && _windows_SANDAL2->current->liste){
        ldc=&(_windows_SANDAL2->current->liste->first);
        while(*ldc && _windows_SANDAL2->current->toDelete){
            lp=&((*ldc)->first);
            while(*lp && _windows_SANDAL2->current->toDelete){
                e=&((*lp)->first);
		prev = NULL;
                while(e && *e && _windows_SANDAL2->current->toDelete){
		    if((*e)->deleted == 1){
                        if((*e)->element->deleted < 3){
                            delDisplayCodeElement((*e)->element, (*ldc)->code);
                        }
			(*e)->element->deleted = 0;
			(*e)->element->deleteCode = 0;
			etmp = *e;
			*e = (*e)->next;
			if(etmp == (*lp)->last)
			    (*lp)->last = prev;
			else if(etmp == (*lp)->first)
			    (*lp)->first = (*lp)->first->next;
			_windows_SANDAL2->current->toDelete--;
			free(etmp);
			etmp = NULL;
		    }else if((*e)->deleted == -1){
			--_windows_SANDAL2->current->toDelete;
			(*e)->deleted = 0;
			e = &((*e)->next);
		    }else{
			switch((*e)->element->deleted){
			case 1: // deleting
			case 2: // changing display code
			    if((*ldc)->code==(*e)->element->deleteCode){
				(*e)->element->deleted = 0;
				(*e)->element->deleteCode = 0;
				etmp=*e;
				delDisplayCode((*e)->element->codes, (*ldc)->code);
				if(!((*e)->element->codes->first)){
				    _freeElement((*e)->element);
				}
				*e=(*e)->next;
				if(etmp == (*lp)->last)
				    (*lp)->last = prev;
				free(etmp);
				_windows_SANDAL2->current->toDelete--;
			    }else{
				prev = *e;
				e=&((*e)->next);
			    }
			    break;
			case 3: // changing plan
			    if((*lp)->code==(*e)->element->deleteCode){
				(*e)->element->deleted = 0;
				(*e)->element->deleteCode = 0;
				etmp=*e;
				*e=(*e)->next;
				if(etmp == (*lp)->last)
				    (*lp)->last = prev;
				else if(etmp == (*lp)->first)
				    (*lp)->first = (*lp)->first->next;
				free(etmp);
				_windows_SANDAL2->current->toDelete--;
			    }else{
				prev = *e;
				e=&((*e)->next);
			    }
			    break;
			default:
			    prev = *e;
			    e=&((*e)->next);
			}
		    }
                }
                if(!(*lp)->first){
                    ptmp=(*lp)->next;
                    if(*lp == _windows_SANDAL2->current->liste->currentPIterator){
                        _windows_SANDAL2->current->liste->currentPIterator = ptmp;
                    }
                    free(*lp);
                    *lp=ptmp;
                }else{
                    lp=&((*lp)->next);
                }
            }
            if(!(*ldc)->first){
                if((*ldc)->code == _windows_SANDAL2->current->displayCode)
                    _windows_SANDAL2->current->current = NULL;
                dctmp=(*ldc)->next;
                if(*ldc == _windows_SANDAL2->current->liste->currentDCIterator){
                    _windows_SANDAL2->current->liste->currentDCIterator = dctmp;
                }
                free(*ldc);
                *ldc=dctmp;
            }else{
                ldc=&((*ldc)->next);
            }
        }
        _windows_SANDAL2->current->toDelete=0;
    }
}

int delElement(Element *e){
    int error = 1;
  
    if(e && _windows_SANDAL2 && _windows_SANDAL2->current){
        e->deleted=1;
        e->parent->toDelete++;
        error = 0;
    }

    return error;
}
/* ------------------------------------------------------- */



/* -------------------------------------------------------
 * Element creation, freeing and informations
 */
void _freeElement(Element *e){
    if(e){
        if(e->codes){
            freeListDisplayCode(e->codes);
        }
#ifndef DEBUG_SDL2_NO_VIDEO
        if(e->image){
            SDL_DestroyTexture(e->image);
        }
#endif
        if(e->entry){
            free(e->entry);
        }
        if(e->data && e->freeData){
            e->freeData(e->data);
        }
        if(e->font){
            freeFont(e->font);
        }
        if(e->interactions){
            freeListPtrElement(e->interactions);
        }
        if(e->hitboxes){
            freeListClickable(e->hitboxes);
        }
        if(e->animation){
            freeListAnimation(e->animation);
        }
        if(e->actions){
            freeListAction(e->actions);
        }
        free(e);
    }    
}

Element* createBlock(double x,double y,double width,double height,int color[4],int displayCode,int plan){
    Element *e = NULL;

    if(_windows_SANDAL2 && _windows_SANDAL2->current){
        e=(Element*)malloc(sizeof(*e));
        if(e){
            e->deleted=0;
            e->deleteCode=0;
            e->selected=0;
            e->x=x;
            e->y=y;
            e->width=width;
            e->height=height;
            e->prX=.5;
            e->prY=.5;
            e->rotation=0.;
	    e->flip = SANDAL2_FLIP_NONE;
            copyColor(e->coulBlock,color);
            e->codes=initListDisplayCode();
            addDisplayCode(e->codes,displayCode,1,plan);
            e->animation=initListAnimation();
            e->freeData=NULL;
            e->events.action=NULL;
            e->events.onClick=NULL;
            e->events.unClick=NULL;
            e->events.keyPress=NULL;
            e->events.keyReleased=NULL;
            e->events.unSelect=NULL;
            e->events.endSprite=NULL;
	    e->events.onMouseMotion=NULL;
	    e->events.unMouseMotion=NULL;
	    e->events.endAction=NULL;
            e->font=NULL;
            e->entry=NULL;
            e->interactions=NULL;
            e->image=NULL;
            e->hitboxes = initListClickable();
            e->data=NULL;
            e->actions = NULL;
            e->elementParent = NULL;
            e->elementChildren.first = NULL;
            e->elementChildren.last = NULL;
            if(addElement(e)){
                _freeElement(e);
                e=NULL;
            }
        }
    }

    return e;
}

Element* createText(double x,double y,double width,double height,double textSize, const char * font,const char * text,int textColor[4],int quality,int displayCode,int plan){
    Element *e=NULL;

    if(_windows_SANDAL2 && _windows_SANDAL2->current){
        e=(Element*)malloc(sizeof(*e));
        if(e){
            e->deleted=0;
            e->deleteCode=0;
            e->selected=0;
            e->x=x;
            e->y=y;
            e->width=width;
            e->height=height;
            e->prX=.5;
            e->prY=.5;
            e->rotation=0.;
	    e->flip = SANDAL2_FLIP_NONE;
            e->textSize=textSize/100.;
            e->animation=initListAnimation();
            e->image=NULL;
            e->entry=NULL;
            e->interactions=NULL;
            e->hitboxes = initListClickable();
            e->codes=NULL;
            e->font=createFont(font,text,textColor,quality);
            e->data=NULL;
            e->actions = NULL;
            if(e->font){
                e->codes=initListDisplayCode();
                addDisplayCode(e->codes,displayCode,1,plan);
                e->coulBlock[0]=-1;
                e->freeData=NULL;
                e->events.action=NULL;
                e->events.onClick=NULL;
                e->events.unClick=NULL;
                e->events.keyPress=NULL;
                e->events.keyReleased=NULL;
                e->events.unSelect=NULL;
                e->events.endSprite=NULL;
		e->events.onMouseMotion=NULL;
		e->events.unMouseMotion=NULL;
                e->events.endAction=NULL;
                e->elementParent = NULL;
                e->elementChildren.first = NULL;
                e->elementChildren.last = NULL;
                if(addElement(e)){
                    _freeElement(e);
                    e=NULL;
                }
            }else{
                _freeElement(e);
                e=NULL;
            }
        }
    }

    return e;
}

Element* createImage(double x,double y,double width,double height,const char *image,int displayCode,int plan){
    Element *e=NULL;
    SDL_Surface *s;

    if(_windows_SANDAL2 && _windows_SANDAL2->current){
        s=IMG_Load(image);
  
        if(s){
            e=(Element*)malloc(sizeof(*e));
            if(e){
                e->deleted=0;
                e->deleteCode=0;
                e->selected=0;
                e->x=x;
                e->y=y;
                e->width=width;
                e->height=height;
                e->prX=.5;
                e->prY=.5;
                e->rotation=0.;
		e->flip = SANDAL2_FLIP_NONE;
                e->animation=initListAnimation();
#ifndef DEBUG_SDL2_NO_VIDEO
                e->image=SDL_CreateTextureFromSurface(_windows_SANDAL2->current->renderer,s);
#else
		e->image = (SDL_Texture *)1;
#endif
                e->codes=initListDisplayCode();
                addDisplayCode(e->codes,displayCode,1,plan);
                e->coulBlock[0]=-1;
                e->coulBlock[3] = 255;
                e->events.action=NULL;
                e->events.onClick=NULL;
                e->events.unClick=NULL;
                e->events.keyPress=NULL;
                e->events.keyReleased=NULL;
                e->events.unSelect=NULL;
                e->events.endSprite=NULL;
		e->events.onMouseMotion=NULL;
		e->events.unMouseMotion=NULL;
                e->events.endAction=NULL;
                e->freeData=NULL;
                e->font=NULL;
                e->entry=NULL;
                e->interactions=NULL;
                e->hitboxes = initListClickable();
                e->data=NULL;
                e->actions = NULL;
                e->elementParent = NULL;
                e->elementChildren.first = NULL;
                e->elementChildren.last = NULL;
                if(addElement(e)){
                    _freeElement(e);
                    e=NULL;
                }
            }
            SDL_FreeSurface(s);
        }
    }

    return e;
}

Element* createImageBlock(double x,double y,double width,double height,int color[4],int displayCode,int plan){
    Element     * e       = createBlock(x, y, width, height, color, displayCode, plan);
    Uint32        rmask;
    Uint32        gmask;
    Uint32        bmask;
    Uint32        amask;
    SDL_Surface * surface;
    int           error;

    if(e){
        e->coulBlock[0] = -1;
        
#       if SDL_BYTEORDER == SDL_BIG_ENDIAN
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x000000ff;
#       else
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0xff000000;
#       endif
        surface = SDL_CreateRGBSurface(0, 10, 10, 32, rmask, gmask, bmask, amask);

        if(surface){
            SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, color[0], color[1], color[2], color[3]));
            error = setImageSurfaceElement(e, surface);
            SDL_FreeSurface(surface);
        }else{
            error = 1;
        }

        if(error){
            _freeElement(e);
            e = NULL;
        }
    }

    return e;
}

Element* createButton(double x,double y,double width,double height,double texteSize,const char * font,const char * text,int textColor[4],int quality,int colorBlock[4],int displayCode,int plan){
    Element *e = NULL;
    Font * f;

    if(_windows_SANDAL2 && _windows_SANDAL2->current){
        e=createBlock(x,y,width,height,colorBlock,displayCode,plan);
        if(e){
            e->textSize=texteSize/100.f;
            f=createFont(font,text,textColor,quality);
            if(f){
                e->font=f;
            }else{
                delElement(e);
                _freeElement(e);
                e=NULL;
            }
        }
    }

    return e;
}

Element* createButtonImage(double x,double y,double width,double height,double texteSize,const char * font,const char * text,int textColor[4],int quality,const char *image,int displayCode,int plan){
    Element *e = NULL;
    Font * f;
  
    if(_windows_SANDAL2 && _windows_SANDAL2->current){
        e=createImage(x,y,width,height,image,displayCode,plan);
        if(e){
            e->textSize=texteSize/100.f;
            f=createFont(font,text,textColor,quality);
            if(f){
                e->font=f;
            }else{
                delElement(e);
                _freeElement(e);
                e=NULL;
            }
        }
    }

    return e;
}

Element* createEntry(double x,double y,double width,double height,double texteSize,const char * font, const char * text,int textColor[4],int quality,int colorBlock[4],int displayCode,int plan,int min,int max,int isScripted){
    Element *e = NULL;
    Entry *ent;
    int i;

    if(_windows_SANDAL2 && _windows_SANDAL2->current){
        e=createButton(x,y,width,height,texteSize,font,text,textColor,quality,colorBlock,displayCode,plan);
        if(e){
            ent=(Entry*)malloc(sizeof(*ent));
            if(ent){
                ent->size_min=min;
                ent->size_max=max;
                ent->isSelect=0;
                ent->isScripted=isScripted;
                PFREE(e->font->text);
                e->font->text=(char*)malloc((max*2+1)*sizeof(*(e->font->text)));
                if(e->font->text){
		    if(text){
			for(i = 0; text[i] && i < max * 2; ++i){
			    e->font->text[i] = text[i];
			}
		        ent->size = i;
		    }else{
			i = 0;
			ent->size = 0;
		    }
                    for(; i < max * 2; ++i){
                        e->font->text[i] = ' ';
                    }
                    e->font->text[max]='\0';
                    e->entry=ent;
                }else{
                    delElement(e);
                    _freeElement(e);
                    e=NULL;
                }
            }else{
                delElement(e);
                _freeElement(e);
                e=NULL;
            }
        }
    }
  
    return e;  
}

Element* createEntryImage(double x,double y,double width,double height,double texteSize,const char * font,const char * text,int textColor[4],int quality,const char *image,int displayCode,int plan,int min,int max,int isScripted){
    Element *e = NULL;
    Entry *ent;
    int i;

    if(_windows_SANDAL2 && _windows_SANDAL2->current){
        e=createButtonImage(x,y,width,height,texteSize,font,text,textColor,quality,image,displayCode,plan);
        if(e){
            ent=(Entry*)malloc(sizeof(*ent));
            if(ent){
                ent->size_min=min;
                ent->size_max=max;
                ent->isSelect=0;
                ent->isScripted=isScripted;
                PFREE(e->font->text);
                e->font->text=(char*)malloc((max*2+1)*sizeof(*(e->font->text)));
                if(e->font->text){
		    if(text){
			for(i = 0; text[i] && i < max * 2; ++i){
			    e->font->text[i] = text[i];
			}
		        ent->size = i;
		    }else{
			i = 0;
			ent->size = 0;
		    }
                    for(; i < max * 2; ++i){
                        e->font->text[i] = ' ';
                    }
                    e->font->text[max]='\0';
                    e->entry=ent;
                }else{
                    delElement(e);
                    _freeElement(e);
                    e=NULL;
                }
            }else{
                delElement(e);
                _freeElement(e);
                e=NULL;
            }
        }
    }
  
    return e;  
}

int isDisplaiedElement(Element *e){
    int display = (e && !(e->deleted) && _windows_SANDAL2 && _windows_SANDAL2->current && _windows_SANDAL2->current == e->parent && e->codes);
    DisplayCode *d;

    if(display){
        d=e->codes->first;
        while(d && d->code<_windows_SANDAL2->current->displayCode){
            d=d->next;
        }
        display=d && d->code==_windows_SANDAL2->current->displayCode && d->isDisplaied;
    }
  
    return display;
}

int clearDisplayCode(int code){
    ListPtrElement * lptr;
    ListDCElement ** ldc;
    PtrElement * ptr;
    int error = 1;

    if(_windows_SANDAL2 && _windows_SANDAL2->current && _windows_SANDAL2->current->liste){
	ldc = &(_windows_SANDAL2->current->liste->first);
	while(*ldc && (*ldc)->code < code)
	    ldc = &((*ldc)->next);
	if(*ldc && (*ldc)->code == code){
	    error = 0;
	    lptr = (*ldc)->first;
	    while(lptr){
		ptr = lptr->first;
		while(ptr){
		    ptr->deleted = 0;
                    ptr->element->deleted=2;
                    ptr->element->deleteCode=code;
                    _windows_SANDAL2->current->toDelete++;
		    ptr = ptr->next;
		}
		lptr = lptr->next;
	    }
	}
    }

    return error;
}

int getFlipStateElement(Element * e,SANDAL2_FLIP * flip){
    if(e && flip)
	*flip = e->flip;

    return !e;
}

int getCoordElement(Element* e,double* x,double* y){
    int error = 1;

    if(e){
        if(x){
            *x=e->x;
        }
        if(y){
            *y=e->y;
        }
        error = 0;
    }

    return error;
}

int getAngleElement(Element* e,double* a){
    int error = 1;

    if(e){
        if(a){
            *a=e->rotation;
        }
        error=0;
    }

    return error;
}

int getDimensionElement(Element* e,double* w,double * h){
    int error = 1;

    if(e){
        if(w){
            *w=e->width;
        }
        if(h){
            *h=e->height;
        }
        error=0;
    }

    return error;
}

int getRotationPointElement(Element* e,double *x,double *y){
    int error = 1;

    if(e){
        if(x){
            *x=e->prX;
        }
        if(y){
            *y=e->prY;
        }
        error = 0;
    }

    return error;
}

int getDataElement(Element* e,void ** data){
    int error = 1;

    if(e){
        error = 0;
        if(data){
            *data=e->data;
        }
    }

    return error;
}

int isSelectedElement(Element *e, int * select){
    int error = 1;

    if(e){
        error = 0;
        if(select){
            *select = e->selected;
        }
    }

    return error;
}

int getTextStyleElement(Element *e,int * style){
    int error = 1;

    if(e && e->font){
        error=getStyleFont(e->font,style);
    }

    return error;
}

int getTextElement(Element *e,char **s){
    int error = 1;

    if(e && e->font && e->font->text){
        if(s){
	    if(e->entry && e->entry->size){
		*s=(char*)malloc((e->entry->size+1)*sizeof(**s));
		if(*s){
		    strncpy(*s,e->font->text,e->entry->size);
		    (*s)[e->entry->size] = 0;
		}
		error = *s && abs(strncmp(*s,e->font->text,e->entry->size));
	    }else{
		*s=(char*)malloc((strlen(e->font->text)+1)*sizeof(**s));
		strcpy(*s,e->font->text);
		error = *s && abs(strcmp(*s,e->font->text));
	    }
        }else{
            error = 0;
        }
    }

    return error;
}

int getColorElement(Element * e, int color[4]){
    int error = 1;

    if(e && color){
	error = 0;
	copyColor(color,e->coulBlock);
    }

    return error;
}

int getWidthElement(Element * e,double * w){
    int error = 1;

    if(e && w){
	error = 0;
	*w = e->width;
    }

    return error;
}

int getHeightElement(Element * e,double * h){
    int error = 1;

    if(e && h){
	error = 0;
	*h = e->height;
    }

    return error;
}

int getCoordXElement(Element * e,double * x){
    int error = 1;

    if(e && x){
	error = 0;
	*x = e->x;
    }

    return error;
}

int getCoordYElement(Element * e,double * y){
    int error = 1;

    if(e && y){
	error = 0;
	*y = e->y;
    }

    return error;
}

int getAlphaElement(Element * e, int * alpha){
    int error = 1;

    if(e && alpha){
	error = 0;
	*alpha = e->coulBlock[3];
    }

    return error;
}
/* ------------------------------------------------------- */



/* -------------------------------------------------------
 * Element modifications
 */
int setFontElement(Element *e,const char * font){
    Font *f;
    int color[4];
    int error = 1;

    if(_windows_SANDAL2 && _windows_SANDAL2->current){
        if(e && font && e->font){
            color[0]=e->font->color.r;
            color[1]=e->font->color.g;
            color[2]=e->font->color.b;
            color[3]=e->font->color.a;
            f=createFont(font,e->font->text,color,e->font->quality);
            if(f){
                freeFont(e->font);
                e->font=f;
                error = 0;
            }
        }
    }

    return error;
}

int setTextElement(Element *e,const char * text){
    int error = 1;
    int size;
    int i;
    
    if(_windows_SANDAL2 && _windows_SANDAL2->current && e && text && e->font){
	if(e->entry){
	    size = strlen(text);
	    if(size > e->entry->size_max){
		strncpy(e->font->text,text,e->entry->size_max);
		e->font->text[e->entry->size_max] = 0;
	    }else{
		strncpy(e->font->text,text,size);
		for(i = size; i < e->entry->size_max - size; ++i){
		    e->font->text[i] = ' ';
		}
		e->font->text[i] = 0;
	    }
	    error = setTextFont(e->font,e->font->text);
	    e->entry->size = (size >= e->entry->size_max ? e->entry->size_max : size);
	}else
	    error=setTextFont(e->font,text);
    }

    return error;
}

int setTextStyleElement(Element *e,int style){
    int error = 1;

    if(e && e->font){
        error=setStyleFont(e->font,style);
    }

    return error;
}

int setColorElement(Element *e,int color[4]){
    int error = 1;
  
    if(e){
        error = 0;
        copyColor(e->coulBlock,color);
    }

    return error;
}

int setTextColorElement(Element *e, int color[4]){
    int error = 1;

    if(e && e->font){
        error=setColorFont(e->font,color);
    }

    return error;
}

int setTextQualityElement(Element *e, int quality){
    int error = 1;

    if(e && e->font){
        error = 0;
        e->font->quality=quality;
    }

    return error;
}

int setImageElement(Element *e,const char *image){
    SDL_Surface *s;
    int error=1;
  
    if(e && _windows_SANDAL2 && _windows_SANDAL2->current){
        if(image){
            s=IMG_Load(image);
            if(s){
#ifndef DEBUG_SDL2_NO_VIDEO
                e->image=SDL_CreateTextureFromSurface(_windows_SANDAL2->current->renderer,s);
#else
		e->image = (SDL_Texture *)1;
#endif
                SDL_FreeSurface(s);
                error=0;
            }
        }else{
#ifndef DEBUG_SDL2_NO_VIDEO
            if(e->image){
                SDL_DestroyTexture(e->image);
            }
#endif
            e->image=NULL;
            error=0;
        }
    }

    return error;
}

int setImageSurfaceElement(Element * e, SDL_Surface * image){
    int           error   = 1;
    SDL_Texture * texture = NULL;

    if(e
#      ifndef DEBUG_SDL2_NO_VIDEO
       && e->parent && e->parent->renderer
#      endif
        ){
#       ifndef DEBUG_SDL2_NO_VIDEO
        if(image){
            texture = SDL_CreateTextureFromSurface(e->parent->renderer, image);
        }
	if(e->image && (texture || image)){
	    SDL_DestroyTexture(e->image);
	}
        if(!image || texture){
            e->image = texture;
        }
        error = image && !texture;
#       else
        error = 0;
        e->image = (SDL_Texture*)image;
#       endif
    }

    return error;
}

int replaceElement(Element *e,double x,double y){
    int error = 1;

    if(e && _windows_SANDAL2 && _windows_SANDAL2->current){
        replaceChildren(e->elementChildren, x - e->x, y - e->y);
        
        e->x = x;
        e->y = y;
        
        error = 0;
    }

    return error;
}

int moveElement(Element *e,double x,double y){
    int error = 1;

    if(e && _windows_SANDAL2 && _windows_SANDAL2->current){
        e->x += x;
        e->y += y;
        
        replaceChildren(e->elementChildren, x, y);
        
        error = 0;
    }

    return error;
}

int setDimensionElement(Element *e,double width,double height){
    int error = 1;

    if(e){
        redimChildren(e->elementChildren, width / e->width, height / e->height);
        e->width=width;
        e->height=height;
        error = 0;
    }

    return error;
}

int setTextSize(Element *e,double textSize){
    int error = 1;

    if(e){
        e->textSize=textSize/100.0f;
        error = 0;
    }

    return error;
}

int addDisplayCodeElement(Element *e,int displayCode, int plan){
    DisplayCode **d, *tmp;
    ListDCElement **ldc, *dctmp;
    ListPtrElement **lp, *ptmp;
    PtrElement *el;
    int error = 1;

    if(_windows_SANDAL2 && _windows_SANDAL2->current && _windows_SANDAL2->current->liste && e && e->codes){
        d=&(e->codes->first);
        while(*d && (*d)->code<displayCode){
            d=&((*d)->next);
        }
        if(!(*d && (*d)->code==displayCode && (*d)->plan==plan)){
            if(*d && (*d)->code == displayCode){
                (*d)->plan=plan;
		error = 0;
            }else{
                tmp=(DisplayCode*)malloc(sizeof(*tmp));
                tmp->code=displayCode;
                tmp->plan = plan;
                tmp->isDisplaied = 1;
                tmp->next=*d;
                *d=tmp;
                ldc=&(_windows_SANDAL2->current->liste->first);
                while(*ldc && (*ldc)->code < displayCode){
                    ldc=&((*ldc)->next);
                }
                if(!*ldc || (*ldc)->code != displayCode){
                    dctmp=initListDCElement(displayCode);
                    if(!_windows_SANDAL2->current->current && _windows_SANDAL2->current->displayCode == displayCode)
                        _windows_SANDAL2->current->current = dctmp;
                    dctmp->next=*ldc;
                    *ldc=dctmp;
                }
                lp=&((*ldc)->first);
                while(*lp && (*lp)->code > plan){
                    lp=&((*lp)->next);
                }
                if((!*lp || (*lp)->code != plan)){
                    ptmp=initListPtrElement(plan);
                    ptmp->next=*lp;
                    *lp=ptmp;
                }
                el=(PtrElement*)malloc(sizeof(*el));
                if(el){
                    error = 0;
                    el->next=NULL;
                    el->element=e;
		    ++e->codes->size;
                    if((*lp)->last){
                        (*lp)->last->next=el;
                    }else{
                        (*lp)->first=el;
                    }
                    (*lp)->last=el;
                }
            }
        }
    }

    return error;
}

int delDisplayCodeElement(Element *e,int displayCode){
    DisplayCode **d, *tmp;
    ListDCElement **ldc;
    ListPtrElement **lp;
    PtrElement **cour;
    int error = 1;

    if(_windows_SANDAL2 && _windows_SANDAL2->current && _windows_SANDAL2->current->liste && e && e->codes){
        d=&(e->codes->first);
        while(*d && (*d)->code < displayCode){
            d=&((*d)->next);
        }
        if(*d && (*d)->code==displayCode){
            ldc=&(_windows_SANDAL2->current->liste->first);
            while(*ldc && (*ldc)->code < displayCode){
                ldc=&((*ldc)->next);
            }
            if(*ldc && (*ldc)->code == displayCode){
                lp=&((*ldc)->first);
                while(*lp && (*lp)->code > (*d)->plan){
                    lp = &((*lp)->next);
                }
                if(*lp && (*lp)->code == (*d)->plan){
                    cour=&((*lp)->first);
                    while(*cour && (*cour)->element!=e){
                        cour=&((*cour)->next);
                    }
                    if(*cour){
                        (*cour)->element->deleted=2;
                        (*cour)->element->deleteCode=displayCode;
                        _windows_SANDAL2->current->toDelete++;
                        error = 0;
                    }
                }
            }
            tmp=(*d)->next;
            free(*d);
            *d=tmp;
        }
    }

    return error;
}

int setDisplayCodeElement(Element *e,int displayCode,int isDisplaied){
    DisplayCode *d;
    int error = 1;

    if(e && e->codes){
        d=e->codes->first;
        while(d && d->code<displayCode){
            d=d->next;
        }
        if(d && d->code==displayCode){
            error = 0;
            d->isDisplaied=isDisplaied;
        }
    }

    return error;
}

int setPlanElement(Element *e,int displayCode,int plan){
    int old;
    DisplayCode *d;
    ListDCElement *ldc;
    ListPtrElement **lp, *ptmp, **lpNew = NULL;
    PtrElement **cour,*tmp;
    int error = 1;

    if(e && _windows_SANDAL2 && _windows_SANDAL2->current && _windows_SANDAL2->current->liste){
        d=e->codes->first;
        while(d && d->code < displayCode){
            d=d->next;
        }
        if(d && d->code==displayCode && d->plan != plan){
            old = d->plan;
            d->plan = plan;
	    /* searching for the list corresponding to the display code */
            ldc=_windows_SANDAL2->current->liste->first;
            while(ldc && ldc->code < displayCode){
                ldc=ldc->next;
            }
            if(ldc && ldc->code == displayCode){
		/* searching for the list corresponding to the old plan */
                lp=&(ldc->first);
                while(*lp && (*lp)->code > old){
                    if((*lp)->code == plan){
			/* if a list corresponding to the new plan is find */
                        lpNew=lp;
                    }
                    lp=&((*lp)->next);
                }
		/* searching for the element */
                cour=&((*lp)->first);
                while(*cour && (*cour)->element!=e){
                    cour=&((*cour)->next);
                }
                if(*cour){ /* if element found */
                    (*cour)->element->deleted=3;
                    (*cour)->element->deleteCode=old;
		    (*cour)->deleted = 1;
                    ++_windows_SANDAL2->current->toDelete;
                }
                if(!lpNew){ /* if list corresponding to new plan not found */
                    lpNew=&(ldc->first);
                    while(*lpNew && (*lpNew)->code > plan){
                        lpNew=&((*lpNew)->next);
                    }
                    if(!*lpNew || (*lpNew)->code != plan){
                        ptmp=initListPtrElement(plan);
                        ptmp->next=*lpNew;
                        *lpNew=ptmp;
                    }
                }
                tmp=(PtrElement*)malloc(sizeof(*tmp));
                if(tmp){
		    tmp->deleted = -1;
                    tmp->element = e;
		    tmp->next = NULL;
		    if((*lpNew)->last){
			(*lpNew)->last->next = tmp;
		    }else{
			(*lpNew)->first = tmp;
		    }
		    (*lpNew)->last = tmp;
                    error=0;
                    ++_windows_SANDAL2->current->toDelete;
                }
            }
        }else if(d && d->code==displayCode && d->plan == plan){
	    error = 0;
	}
    }

    return error;
}

int clearPlanDisplayCode(int code, int plan){
    ListDCElement * ldc;
    ListPtrElement * lptr;
    PtrElement * ptr;
    int error = 1;

    if(_windows_SANDAL2 && _windows_SANDAL2->current && _windows_SANDAL2->current->liste){
	ldc = _windows_SANDAL2->current->liste->first;
	while(ldc && ldc->code < code)
	    ldc = ldc->next;
	if(ldc && ldc->code == code){
	    lptr = ldc->first;
	    while(lptr && lptr->code > plan)
		lptr = lptr->next;
	    if(lptr && lptr->code == plan){
		error = 0;
		ptr = lptr->first;
		while(ptr){
                    ptr->element->deleted=3;
                    ptr->element->deleteCode=plan;
                    _windows_SANDAL2->current->toDelete++;
		    ptr = ptr->next;
		}
	    }
	}
    }

    return error;
}

int setActionElement(Element *e,void (*action)(Element*)){
    int error = 1;
  
    if(e){
        error=0;
        e->events.action=action;
    }

    return error;
}

int setKeyPressedElement(Element *e,void (*keyPress)(Element*,SDL_Keycode c)){
    int error = 1;
  
    if(e){
        error=0;
        e->events.keyPress=keyPress;
    }

    return error;
}

int setKeyReleasedElement(Element *e,void (*keyReleased)(Element*,SDL_Keycode c)){
    int error = 1;
  
    if(e){
        error=0;
        e->events.keyReleased=keyReleased;
    }

    return error;
}

int setUnClickElement(Element *e,void (*unCLick)(Element*, int)){
    int error = 1;
  
    if(e){
        error=0;
        e->events.unClick=unCLick;
    }

    return error;
}

int setOnClickElement(Element *e,void (*onCLick)(Element*, int)){
    int error = 1;
  
    if(e){
        error=0;
        e->events.onClick=onCLick;
    }

    return error;
}

int setOnMouseMotionElement(Element *e, void (*onMouseMotion)(Element*)){
  int error = 1;

  if(e){
    error = 0;
    e->events.onMouseMotion=onMouseMotion;
  }

  return error;
}

int setUnMouseMotionElement(Element *e, void (*unMouseMotion)(Element*)){
  int error = 1;

  if(e){
    error = 0;
    e->events.unMouseMotion=unMouseMotion;
  }

  return error;
}

int setUnSelectElement(Element *e,void (*unSelect)(Element*)){
    int error = 1;
  
    if(e){
        error=0;
        e->events.unSelect=unSelect;
    }

    return error;
}

int setEndSpriteElement(Element *e,void (*endSprite)(Element*,int)){
    int error = 1;
  
    if(e){
        error=0;
        e->events.endSprite=endSprite;
    }

    return error;
}

int setEndActionElement(Element *e,void (*endAction)(Element*)){
    int error = 1;
  
    if(e){
        error=0;
        e->events.endAction = endAction;
    }

    return error;
}

int addElementToElement(Element *e,Element *add){
    PtrElement *pe;
    int error = 1;
  
    if(e && add){
        pe=(PtrElement*)malloc(sizeof(*pe));
        pe->element=add;
        pe->next=NULL;
    
        if(!e->interactions){
            e->interactions=(ListPtrElement*)malloc(sizeof(*(e->interactions)));
            if(e->interactions){
                e->interactions->first=pe;
                e->interactions->last=pe;
                e->interactions->current=pe;
                error = 0;
            }
        }else{
            if(!e->interactions->first){
                e->interactions->first=pe;
                e->interactions->current=pe;
            }else{
                e->interactions->last->next=pe;
            }
            e->interactions->last=pe;
            error = 0;
        }
    }

    return error;
}

int delElementToElement(Element *e,Element *del){
    PtrElement **pe, *rm, **tmp=NULL;
    int error = 1;

    if(e && e->interactions && del){
        pe=&(e->interactions->first);

        while(*pe && (*pe)->element!=del){
            tmp=pe;
            pe=&((*pe)->next);
        }

        if(*pe && (*pe)->element==del){
            error = 0;
            rm=*pe;
            *pe=(*pe)->next;
            free(rm);
            if(rm == e->interactions->current){
                e->interactions->current=*pe;
            }
            if(rm == e->interactions->last){
                if(tmp){
                    e->interactions->last=*tmp;
                }else{
                    e->interactions->last=NULL;
                }
            }
        }
    }

    return error;
}

int clearElementToElement(Element * e){
    PtrElement * pe, * tmp;

    if(e && e->interactions){
	pe = e->interactions->first;

	while(pe){
	    tmp = pe->next;
	    free(pe);
	    pe = tmp;
	}

	e->interactions->first   = NULL;
	e->interactions->last    = NULL;
	e->interactions->current = NULL;
    }

    return !e;
}

int addClickableElement(Element *e,Clickable *hb,int blocking){
    int error = 1;

    if(e && hb){
        error=addClickable(e->hitboxes,hb,blocking);
    }

    return error;
}

int addAngleElement(Element *e,double a){
    int error = 1;

    if(e && e->coulBlock[0]==-1){
        e->rotation += a;
        rotateChildren(e->elementChildren, a);
        if(e->rotation > 360){
            e->rotation -= 360;
        }
        error = 0;
    }

    return error;
}

int rotateElement(Element * e, double a, double prX, double prY){
    double x, y, newX, newY;
    double c, s, oldA, hyp;
    int error = 1;

    if(e){
        error = 0;

        x = e->prX * e->width + e->x;
        y = e->prY * e->height + e->y;

        if(prX != x || prY != y){
            if(prX != x){
                oldA = atan((y - prY) / (x - prX));
            }else{
                oldA = M_PI / 2;
            }
            if(x < prX){
                if(y < prY){
                    oldA = oldA - M_PI;
                }else{
                    oldA = M_PI + oldA;
                }
            }

            hyp = sqrt((prX - x)*(prX - x) + (prY - y)*(prY - y));
            newX = prX + hyp*cos(oldA + M_PI * a / 180);
            newY = prY + hyp*sin(oldA + M_PI * a / 180);
            
            newX = newX - e->prX * e->width;
            newY = newY - e->prY * e->height;

            x = e->x;
            y = e->y;

            e->x = newX;
            e->y = newY;
                
            replaceChildren(e->elementChildren, newX - x, newY - y);
            rotateChildren(e->elementChildren, a);
        }
            
        e->rotation += a;
    }

    return error;
}

int setAngleElement(Element *e,double a){
    int error = 1;

    if(e && e->coulBlock[0]==-1){
        error = addAngleElement(e, a - e->rotation);
    }

    return error;
}

int setRotationPointElement(Element *e,double x,double y){
    int error = 1;

    if(e && e->coulBlock[0]==-1){
        e->prX=x;
        e->prY=y;
        error = 0;
    }

    return error;
}

int setDataElement(Element *e,void *data){
    int error = 1;

    if(e){
        error = 0;
        e->data=data;
    }

    return error;
}

int setFreeDataElement(Element *e,void (*freeData)(void*)){
    int error = 1;

    if(e){
        e->freeData=freeData;
        error = 0;
    }

    return error;
}

int freeDataElement(Element *e){
    int error = 1;

    if(e && e->data && e->freeData){
        e->freeData(e->data);
        e->data=NULL;
        error=0;
    }

    return error;
}

int addAnimationElement(Element *e,int code){
    int error = 1;

    if(e){
        error=createAnimation(e->animation,code);
    }

    return error;
}

int delAnimationElement(Element *e,int code){
    int error = 1;

    if(e){
        error=delAnimation(e->animation,code);
    }

    return error;
}

int addSpriteAnimationElement(Element *e,int code,int x,int y,int width,int height,int lifespan,int codeS){
    int error = 1;

    if(e){
        error = addSpriteAnimation(e->animation,code,x,y,width,height,lifespan,codeS);
    }
  
    return error;
}

int delSpriteAnimationElement(Element *e,int code,int codeS){
    int error = 1;

    if(e){
        error = delSpriteAnimation(e->animation,code,codeS);
    }

    return error;
}

int setLifeSpanSpriteAnimationElement(Element * e,int code,int codeS,unsigned lifespan){
    int error = 1;

    if(e){
        error = setLifeSpanSpriteAnimation(e->animation,code,codeS,lifespan);
    }

    return error;
}

int nextSpriteElement(Element * e){
    int error = 1;

    if(e && e->animation->size && e->animation->current->size){
        e->animation->current->current = e->animation->current->current->next;
        e->animation->current->wasChanged=0;
        error = 0;
    }

    return error;
}

int previousSpriteElement(Element * e){
    int error = 1;

    if(e && e->animation->size && e->animation->current->size){
        e->animation->current->current = e->animation->current->current->prev;
        e->animation->current->wasChanged=0;
        error = 0;
    }

    return error;
}

int setWaySpriteAnimationElement(Element * e,int code, int side){
    int error = 1;
    ListSprite *ls;
    unsigned i=0;

    if(e && (side == 1 || !side || side == -1) && e->animation->size){
        ls=e->animation->first;
        while(i<e->animation->size && ls->code != code){
            ls=ls->next;
            ++i;
        }
        if(i<e->animation->size){
            error = 0;
            ls->side=side;
        }
    }

    return error;
}

int nextAnimationElement(Element * e){
    int error=1;

    if(e && e->animation->size){
        e->animation->current=e->animation->current->next;
        error = 0;
    }

    return error;
}

int previousAnimationElement(Element * e){
    int error=1;

    if(e && e->animation->size){
        e->animation->current=e->animation->current->prev;
        error = 0;
    }

    return error;
}

int setAnimationElement(Element *e,int code){
    int error = 1;

    if(e && e->animation){
        error = setAnimation(e->animation,code);
    }

    return error;
}

int setSpriteAnimationElement(Element *e,int codeS){
    int error = 1;

    if(e && e->animation){
        error = setSpriteAnimation(e->animation,codeS);
    }

    return error;
}

int clearWindow(void){
    ListDCElement * ldc, * ltmp;
    int error = 1;

    if(_windows_SANDAL2 && _windows_SANDAL2->current && _windows_SANDAL2->current->liste){
	error = 0;

	ldc = _windows_SANDAL2->current->liste->first;
	while(ldc){
	    ltmp = ldc->next;
	    freeListDCElement(ldc);
	    ldc = ltmp;
	}
	_windows_SANDAL2->current->stop = 1;
	_windows_SANDAL2->current->liste->first = NULL;
	_windows_SANDAL2->current->liste->currentDCIterator = NULL;
	_windows_SANDAL2->current->liste->currentPIterator = NULL;
    }

    return error;
}

int setFlipStateElement(Element * e, SANDAL2_FLIP flip){
    if(e)
	e->flip = flip;

    return !e;
}

int setWidthElement(Element * e, double width){
    if(e){
        redimChildren(e->elementChildren, width / e->width, 1);
	e->width = width;
    }

    return !e;
}

int setHeightElement(Element * e, double height){
    if(e){
        redimChildren(e->elementChildren, 1, height / e->height);
	e->height = height;
    }

    return !e;
}

int setCoordXElement(Element * e, double x){
    if(e){
        replaceChildren(e->elementChildren, x - e->x, 0);
	e->x = x;
    }

    return !e;
}

int setCoordYElement(Element * e, double y){
    if(e){
        replaceChildren(e->elementChildren, 0, y - e->y);
	e->y = y;
    }

    return !e;
}

int setActionListElement(Element * e, ListAction * actions){
    if(e){
        if(e->actions){
            freeListAction(e->actions);
        }
        if(actions && !actions->isParallel){
#ifndef DEBUG_SDL2_NO_VIDEO
            actions = generateParallelAction(actions, NULL);
#endif
        }
        e->actions = actions;
    }

    return !e;
}

int setAlphaElement(Element * e, int alpha){
    int error = 1;

    if(e){
        error = 0;

        if(alpha < 0){
            alpha = 0;
        }else if(alpha > 255){
            alpha = 255;
        }

        if(alpha != e->coulBlock[3] && e->image){
            error = SDL_SetTextureAlphaMod(e->image, alpha);
        }

        e->coulBlock[3] = alpha;
    }

    return error;
}

long long addActionToElement(Element * e, ListAction * action){
    long long id = -1;

    if(e && action){
        if(!e->actions){
            e->actions = generateParallelAction(action, NULL);
            id = 0;
        }else{
            id = addActionAtEndAction(e->actions, action);
        }
    }

    return id;
}

int delActionToElement(Element * e, long long index){
    int error = 1;

    if(e){
        error = delActionToAction(e->actions, index);
    }

    return error;
}

int setParentElement(Element * parent, Element * child){
    int error = 1;
    PtrElement ** pE, * next;

    if(parent && child){
        error = 0;

        if(child->elementParent != parent){
            if(child->elementParent){
                error = delParentElement(child);
            }

            if(!error){
                next = (PtrElement*)malloc(sizeof(PtrElement));

                if(!next){
                    error = 1;
                }else{
                    pE = &(parent->elementChildren.last);
                    if(*pE){
                        pE = &((*pE)->next);
                    }else{
                        parent->elementChildren.first = next;
                    }
                    next->next = NULL;
                    next->element = child;
                    *pE = next;
                    parent->elementChildren.last = next;

                    child->elementParent = parent;
                }
            }
        }
    }

    return error;
}

int delParentElement(Element * child){
    int error = 1;
    PtrElement ** pE, * tmp;
    
    if(child){
        error = 0;

        if(child->elementParent){
            pE = &(child->elementParent->elementChildren.first);
            while(*pE && (*pE)->element != child){
                pE = &((*pE)->next);
            }

            if(*pE){
                tmp = *pE;
                *pE = (*pE)->next;
                free(tmp);
            }

            if(!child->elementParent->elementChildren.first){
                child->elementParent->elementChildren.last = NULL;
            }
            child->elementParent = NULL;
        }
    }
    
    return error;
}
/* ------------------------------------------------------- */



/* -------------------------------------------------------
 * Element's list binded to an Element's iterator
 */
int initIteratorElement(Element *e){
    int succes = 0;
  
    if(e && e->interactions && e->interactions->first){
        e->interactions->current=e->interactions->first;
        succes = 1;
    }

    return succes;
}

Element* nextIteratorElement(Element *e){
    Element* next=NULL;
  
    if(e && e->interactions && e->interactions->current){
        next=e->interactions->current->element;
        e->interactions->current=e->interactions->current->next;
    }

    return next;
}
/* ------------------------------------------------------- */



/* ------------------------------------------------------- 
 * Element's list iterator
 */
int initIterator(int displayCode){
    int succes = 0;
    ListDCElement *lp;
  
    if(_windows_SANDAL2 && _windows_SANDAL2->current && _windows_SANDAL2->current->liste){
        if(_windows_SANDAL2->current->liste->currentDCIterator && _windows_SANDAL2->current->liste->currentDCIterator->code == displayCode){
            _windows_SANDAL2->current->liste->currentPIterator=_windows_SANDAL2->current->liste->currentDCIterator->first;
            _windows_SANDAL2->current->liste->currentPIterator->current=_windows_SANDAL2->current->liste->currentPIterator->first;
            succes=1;
        }else{
            if(_windows_SANDAL2->current->liste->currentDCIterator && _windows_SANDAL2->current->liste->currentDCIterator->code < displayCode){
                lp=_windows_SANDAL2->current->liste->currentDCIterator;
            }else{
                lp=_windows_SANDAL2->current->liste->first;
            }
            while(lp->code < displayCode){
                lp=lp->next;
            }
            if(lp && lp->code==displayCode){
                _windows_SANDAL2->current->liste->currentDCIterator=lp;
                _windows_SANDAL2->current->liste->currentPIterator=lp->first;
                lp->first->current=lp->first->first;
                succes=1;
            }
        }
    }

    return succes;
}

Element* nextElement(){
    Element *res = NULL;
    PtrElement *pres;

    if(_windows_SANDAL2 && _windows_SANDAL2->current && _windows_SANDAL2->current->liste && _windows_SANDAL2->current->liste->currentDCIterator){
        if(_windows_SANDAL2->current->liste->currentPIterator){
            pres=_windows_SANDAL2->current->liste->currentPIterator->current;
            if(pres){
                _windows_SANDAL2->current->liste->currentPIterator->current = _windows_SANDAL2->current->liste->currentPIterator->current->next;
                res=pres->element;
            }else{
                _windows_SANDAL2->current->liste->currentPIterator = _windows_SANDAL2->current->liste->currentPIterator->next;
                if(_windows_SANDAL2->current->liste->currentPIterator){
                    _windows_SANDAL2->current->liste->currentPIterator->current=_windows_SANDAL2->current->liste->currentPIterator->first->next;
                    res=_windows_SANDAL2->current->liste->currentPIterator->first->element;
                }
            }
        }
    }

    return res;
}
/* ------------------------------------------------------- */



/* -------------------------------------------------------
 * Entry modifications
 */
int setSizeEntry(Element *e,int size_min,int size_max){
    char *s;
    int error = 1;
  
    if(_windows_SANDAL2 && _windows_SANDAL2->current && e && e->entry){
        error = 0;
        if(size_min>-1 && size_min<=size_max){
            e->entry->size_min=size_min;
        }
        if(size_max>-1 && size_max>=size_min && size_max!=e->entry->size_max){
            s=(char*)malloc((size_max+1)*sizeof(*s));
            error = 1;
            if(s){
                strncpy(s,e->font->text,size_max);
                s[size_max]='\0';
                PFREE(e->font->text);
                e->font->text=s;
                e->entry->size_max=size_max;
                error=actualizeTextFont(e->font,e->entry->isScripted);
            }
        }
    }

    return error;
}

int setScriptedEntry(Element *e,int isScripted){
    int error = 1;
  
    if(_windows_SANDAL2 && _windows_SANDAL2->current && e && e->entry && e->entry->isScripted!=isScripted){
        e->entry->isScripted=isScripted;
        error=actualizeTextFont(e->font,e->entry->isScripted);
    }

    return error;
}

int addCharEntry(Element *e,char c){
    int error = 1;
  
    if(_windows_SANDAL2 && _windows_SANDAL2->current && e && e->entry && e->font && e->font->text && e->entry->size < e->entry->size_max){
        e->font->text[e->entry->size]=c;
	e->font->text[e->entry->size_max*2 - e->entry->size] = 0;
        ++e->entry->size;
        error=actualizeTextFont(e->font,e->entry->isScripted);
    }

    return error;
}

int delCharEntry(Element *e){
    int error = 1;
  
    if(_windows_SANDAL2 && _windows_SANDAL2->current && e && e->entry && e->font && e->font->text && e->entry->size){
        --e->entry->size;
	if(e->entry->size < e->entry->size_max*2 - e->entry->size)
	    e->font->text[e->entry->size_max*2 - e->entry->size] = ' ';
        e->font->text[e->entry->size]=' ';
        error=actualizeTextFont(e->font,e->entry->isScripted);
    }

    return error;
}
/* ------------------------------------------------------- */
