#ifndef CLICKABLE_SANDAL2_KLEVH
#define CLICKABLE_SANDAL2_KLEVH

#ifdef __cplusplus
extern "C" {
#endif
    
#include <math.h>
#include <stdlib.h>

/**
 * @file Clickable.h
 * @author Baptiste PRUNIER (KLEVH)
 * @brief Generic hitboxes either polygonial, elliptic or a mixe of both
 */

/**
 * @struct Line
 * @brief Represente a line which delimitate a hittable surface from a none hittable one
 */
typedef struct Line{
  struct Line * next;
  /**< next element*/
  double a;
  /**< a in the equation of the line ax+by=c*/
  double b;
  /**< b in the equation of the line ax+by=c*/
  double c;
  /**< c in the equation of the line ax+by=c*/
  int side;
  /**< side of the line that is hittable (-1 : ax+by<c, 1 : ax+by>c)*/
}Line;

/**
 * @struct Circle
 * @brief Represente a hittable ellipse
 */
typedef struct Circle{
  struct Circle * next;
  /**< next ellipse*/
  double x;
  /**< abscissa coordinate of the center of the ellipse*/
  double y;
  /**< ordinate coordinate of the center of the ellipse*/
  double rY;
  /**< size of the radius parallel to the ordinate axis*/
  double rX;
  /**< size of the radius parallel to the abscissa axis*/
  double theta;
  /**< degree of rotation*/
}Circle;

/**
 * @struct Clickable
 * @brief Represente a hit boxe which is a collection of Line (defining a convexe polygon) and Circle
 */
typedef struct Clickable{
  Line * firstL;
  /**< first element of the lines' list*/
  Circle * firstC;
  /**< first element of the ellipses' list*/
  struct Clickable * next;
  /**< next hitbox*/
}Clickable;

/**
 * @struct ListClickable
 * @brief Collection of hit boxes
 */
typedef struct{
  Clickable * first;
  /**<First hit box of the list*/
  Clickable * current;
  /**<Current hit box of the list*/
  Clickable * firstBlock;
  /**<First blocking hit box of the list*/
  Clickable * currentBlock;
  /**<Current blocking hit box of the list*/
}ListClickable;

/* ----------------------------------------------------------
 * Clickable
 */
/**
 * @brief Generate an empty hitbox
 * @return return an empty hitbox
 */
Clickable * initClickable();
/**
 * @brief add a line to the hitbox side*(ax+by) > side*c 
 * @param hb : hit boxe to which the line will be added
 * @param a : a in the equation of the line ax+by=c
 * @param b : b in the equation of the line ax+by=c
 * @param c : c in the equation of the line ax+by=c
 * @param side : indicator of the side of the line which is hittable (-1 : ax+by<c, 1 : ax+by>c)
 * @return 0 if the line was correctly added, 1 in the other case
 */
int addLine(Clickable * hb,double a,double b,double c,int side);
/**
 * @brief add an ellipse to the hitbox
 * @param hb : hit boxe to which the line will be added
 * @param x : abscissa coordinate of the center of the ellipse
 * @param y : ordinate coordinate of the center of the ellipse
 * @param rX : size of the radius parallel to the abscissa axis
 * @param rY : size of the radius parallel to the ordinate axis
 * @param theta : rotation of the ellipse (in degree)
 * @return 0 if the ellipse was correctly added, 1 in the other case
 */
int addCircle(Clickable * hb,double x,double y,double rX,double rY,double theta);
/**
 * @brief remove a line from the hitbox 
 * @param hb : hit boxe to which the line will be removed
 * @param a : a in the equation of the line ax+by=c
 * @param b : b in the equation of the line ax+by=c
 * @param c : c in the equation of the line ax+by=c
 * @param side : indicator of the side of the line which is hittable (-1 : ax+by<c, 1 : ax+by>c)
 * @return 0 if the line was correctly removed, 1 in the other case
 */
int delLine(Clickable * hb,double a,double b,double c,int side);
/**
 * @brief remove an ellipse from the hitbox
 * @param hb : hit boxe to which the line will be removed
 * @param x : abscissa coordinate of the center of the ellipse
 * @param y : ordinate coordinate of the center of the ellipse
 * @param rX : size of the radius parallel to the abscissa axis
 * @param rY : size of the radius parallel to the ordinate axis
 * @param theta : rotation of the ellipse (in degree)
 * @return 0 if the ellipse was correctly removed, 1 in the other case
 */
int delCircle(Clickable * hb,double x,double y,double rX,double rY,double theta);
/**
 * @brief tells if (x,y) touch the hitbox 
 * @param hb : hit boxe that you are trying to touch
 * @param x : abscissa coordinate of the point trying to touch the hit boxe
 * @param y : ordinate coordinate of the point trying to touch the hit boxe
 * @return 1 if the hit boxe is hit, 0 in the other case
 */
int hitClickable(Clickable * hb,double x,double y);
/**
 * @brief generate a rectangulare shapped Clickable 
 * @param x1 : abscissa coordinate of the top left point of the rectangle
 * @param y1 : ordinate coordinate of the top left point of the rectangle
 * @param x2 : abscissa coordinate of the bottom right point of the rectangle
 * @param y2 : ordinate coordinate of the bottom right point of the rectangle
 * @return A rectangulare hit boxe defined by the points (x1,y1) and (x2,y2)
 */
Clickable * rectangleClickable(double x1,double y1,double x2,double y2);
/**
 * @brief generate an ellipse shapped Clickable 
 * @param x : abscissa coordinate of the center of the ellipse
 * @param y : ordinate coordinate of the center of the ellipse
 * @param rX : size of the radius parallel to the abscissa axis
 * @param rY : size of the radius parallel to the ordinate axis
 * @param theta : rotation of the ellipse (in degree)
 * @return An elliptical hit boxe
 */
Clickable * ellipseClickable(double x,double y,double rX,double rY,double theta);
/**
 * @brief free a hit boxe's memory
 * @param hb : hit boxe to be freed
 */
void freeClickable(Clickable * hb);
/* ---------------------------------------------------------- */


/* ----------------------------------------------------------
 * ListClickable
 */
/**
 * @brief free the memory of a list of hit boxe
 * @param l : list of hit boxe to be freed
 */
void freeListClickable(ListClickable * l);
/**
 * @brief initialise an empty list of hit boxes
 * @return an empty list of hit boxes
 */
ListClickable * initListClickable();
/**
 * @brief add a hit boxe to a list of hit boxes
 * @param l : list of hit boxes
 * @param hb : hit boxe
 * @return 0 if the hit boxe was succesfully added, 1 if not
 */
int addClickable(ListClickable * l,Clickable * hb,int blocking);
/**
 * @brief remove a hit boxe to a list of hit boxes
 * @param l : list of hit boxes
 * @param hb : hit boxe
 * @param blocking : 0 if the hit boxe is hittable, 1 if it block other hit boxes
 * @return 0 if the hit boxe was succesfully removed, 1 if not
 */
int delClickable(ListClickable * l,Clickable * hb, int blocking);
/**
 * @brief initialise the iterator of the list of non blocking hit boxes
 * @param l : list of hit boxes
 * @return 1 if the iterator was succesfully initialised, 0 if not
 */
int initIteratorListClickable(ListClickable * l);
/**
 * @brief getter of the current non blocking element of the iterator and setter of the next element
 * @return the current hit boxes
 */
Clickable * nextClickable(ListClickable * l);
/**
 * @brief initialise the iterator of the list of blocking hit boxes
 * @param l : list of hit boxes
 * @return 1 if the iterator was succesfully initialised, 0 if not
 */
int initIteratorListClickableBlocking(ListClickable * l);
/**
 * @brief getter of the current blocking element of the iterator and setter of the next element
 * @return the current hit boxes
 */
Clickable * nextClickableBlocking(ListClickable * l);
/**
 * @brief tells if (x,y) touch one of the hitboxes 
 * @param l : list of hit boxes
 * @param x : abscissa coordinate of the point trying to touch the hit boxe
 * @param y : ordinate coordinate of the point trying to touch the hit boxe
 * @return 1 if one of the hit boxes was hit, 0 if not
 */
int hitListClickable(ListClickable * l,double x,double y);
/* ---------------------------------------------------------- */

#ifdef __cplusplus
}
#endif
    
#endif
