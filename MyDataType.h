#ifndef __MYDATATYPE_H__
#define __MYDATATYPE_H__
#include "CNCData.h"
enum   VIEWMODE {NO=0,MAX,MIN,ALL,LK};
enum   VIEWTYPE {VIEWNO=0,VIEW2D,VIEW3D};
enum   FILETYPE {PLT,CNC,FILETYPE_NO};
typedef CArray <CCNCData,CCNCData&> CCncArray;
#endif