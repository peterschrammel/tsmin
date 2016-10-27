/******************************************************************************/
/* TSmin Test Suite Minimization                                              */
/* Author: Peter Schrammel                                                    */
/* Version: 0.2                                                               */
/*                                                                            */
/* Example instantiations                                                     */
/******************************************************************************/

#ifndef TSMIN_TSMIN_H
#define TSMIN_TSMIN_H

#include "../tsmin_template.h"
#include <string>

typedef tsmin_templatet<unsigned,unsigned,unsigned> tsmin_intt;
typedef tsmin_templatet<std::string,unsigned,unsigned> tsmin_strintt;
typedef tsmin_templatet<std::string,std::string,unsigned> tsmin_strstrt;

#endif
