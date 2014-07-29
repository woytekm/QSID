/***************************************************************************
                          null.h  -  NULL audio driver used for hardsid
                                     and songlength detection
                             -------------------
    begin                : Mon Nov 6 2000
    copyright            : (C) 2000 by Simon White
    email                : s_a_white@email.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
 *  $Log: null.h,v $
 *  Revision 1.4  2001/10/30 23:35:35  s_a_white
 *  Added pause support.
 *
 *  Revision 1.3  2001/07/03 17:54:50  s_a_white
 *  Support for new audio interface for better compatibility.
 *
 *  Revision 1.2  2001/01/18 18:36:16  s_a_white
 *  Support for multiple drivers added.  C standard update applied (There
 *  should be no spaces before #)
 *
 *  Revision 1.1  2001/01/08 16:41:43  s_a_white
 *  App and Library Seperation
 *
 ***************************************************************************/

#ifndef audio_null_h_
#define audio_null_h_

#include "config.h"
#ifdef   HAVE_HARDSID
#   ifndef AudioDriver
#   define AudioDriver Audio_Null
#   endif
#endif

#include "../AudioBase.h"

class Audio_Null: public AudioBase
{    
private:  // ------------------------------------------------------- private
    bool isOpen;

public:  // --------------------------------------------------------- public
    Audio_Null();
    ~Audio_Null();

    void *open  (AudioConfig &cfg, const char *);
    void  close ();    
    void *reset ();
    void *write ();
    void  pause () {;}
};

#endif // audio_null_h_
