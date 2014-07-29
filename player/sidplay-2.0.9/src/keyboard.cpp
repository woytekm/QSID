/***************************************************************************
                          keyboard.cpp  -  Keyboard decoding
                             -------------------
    begin                : Thur Dec 7 2000
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
 *  $Log: keyboard.cpp,v $
 *  Revision 1.8  2004/02/21 11:24:44  s_a_white
 *  Don't lockup when the keyboard input stream is not a terminal.
 *
 *  Revision 1.7  2002/10/17 21:23:39  s_a_white
 *  Added string.h as some platforms have bzero using memset without having
 *  a valid prototype available.
 *
 *  Revision 1.6  2001/12/07 18:22:57  s_a_white
 *  Added up/down arrow keys for windows.
 *
 *  Revision 1.5  2001/11/16 19:29:37  s_a_white
 *  Warning fixes.
 *
 *  Revision 1.4  2001/07/25 17:10:41  s_a_white
 *  Added 8/2 to speed up/slow down player.
 *
 *  Revision 1.3  2001/07/03 17:47:56  s_a_white
 *  Added A_NONE.
 *
 *  Revision 1.1  2001/01/08 16:41:42  s_a_white
 *  App and Library Seperation
 *
 *  Revision 1.1  2000/12/12 19:13:15  s_a_white
 *  New keyboard handling routines.
 *
 ***************************************************************************/

#include "keyboard.h"

#ifdef HAVE_UNIX
// Unix console headers
#   include <ctype.h>
// bzero requires memset on some platforms
#   include <string.h>
#   include <termios.h>
#   include <sys/time.h>
#   include <sys/types.h>
#   include <sys/stat.h>
#   include <fcntl.h>
#   include <unistd.h>
char _getch (void);
#endif

#define MAX_CMDLEN 10
#define ESC '\033'

// Special Extended Key Definitions
enum
{
    PCK_HOME          = '\107',
    PCK_UP            = '\110',
    PCK_LEFT          = '\113',
    PCK_RIGHT         = '\115',
    PCK_END           = '\117',
    PCK_DOWN          = '\120',
    PCK_EXTENDED      = '\340'
};

static char keytable[] =
{
    // Windows Special Cursors
#ifdef HAVE_MSWINDOWS
    PCK_EXTENDED, PCK_RIGHT,0,    A_RIGHT_ARROW,
    PCK_EXTENDED, PCK_LEFT,0,     A_LEFT_ARROW,
    PCK_EXTENDED, PCK_UP,0,       A_UP_ARROW,
    PCK_EXTENDED, PCK_DOWN,0,     A_DOWN_ARROW,
    PCK_EXTENDED, PCK_HOME,0,     A_HOME,
    PCK_EXTENDED, PCK_END,0,      A_END,
#endif

#ifdef HAVE_UNIX
    // Linux Special Keys
    ESC,'[','C',0,          A_RIGHT_ARROW,
    ESC,'[','D',0,          A_LEFT_ARROW,
    ESC,'[','A',0,          A_UP_ARROW,
    ESC,'[','B',0,          A_DOWN_ARROW,
    // Hmm, in consile there:
    ESC,'[','1','~',0,      A_HOME,
    ESC,'[','4','~',0,      A_END,
    // But in X there:
    ESC,'[','H',0,          A_HOME,
    ESC,'[','F',0,          A_END,

    ESC,'[','1','0',0,      A_INVALID,
    ESC,'[','2','0',0,      A_INVALID,
#endif

    // General Keys
    '6',0,                  A_RIGHT_ARROW,
    '4',0,                  A_LEFT_ARROW,
    '8',0,                  A_UP_ARROW,
    '2',0,                  A_DOWN_ARROW,
    '7',0,                  A_HOME,
    '1',0,                  A_END,
    'p',0,                  A_PAUSED,
    'P',0,                  A_PAUSED,
    ESC,ESC,0,              A_QUIT,

    // Old Keys
    '>',0,                  A_RIGHT_ARROW,
    '<',0,                  A_LEFT_ARROW,
    '.',0,                  A_RIGHT_ARROW,
    ',',0,                  A_LEFT_ARROW,

    0,                      A_END_LIST
};


/*
 * Search a single command table for the command string in cmd.
 */
static int keyboard_search (char *cmd)
{
    register char *p;
    register char *q;
    register int   a;

    for (p = keytable, q = cmd;;  p++, q++)
    {
        if (*p == *q)
        {
            /*
             * Current characters match.
             * If we're at the end of the string, we've found it.
             * Return the action code, which is the character
             * after the null at the end of the string
             * in the command table.
             */
            if (*p == '\0')
            {
                a = *++p & 0377;
                while (a == A_SKIP)
                    a = *++p & 0377;
                if (a == A_END_LIST)
                {
                    /*
                     * We get here only if the original
                     * cmd string passed in was empty ("").
                     * I don't think that can happen,
                     * but just in case ...
                     */
                    break;
                }
                return (a);
            }
        } else if (*q == '\0')
        {
            /*
             * Hit the end of the user's command,
             * but not the end of the string in the command table.
             * The user's command is incomplete.
             */
            return (A_PREFIX);
        } else
        {
            /*
             * Not a match.
             * Skip ahead to the next command in the
             * command table, and reset the pointer
             * to the beginning of the user's command.
             */
            if (*p == '\0' && p[1] == A_END_LIST)
            {
                /*
                 * A_END_LIST is a special marker that tells 
                 * us to abort the cmd search.
                 */
                break;
            }
            while (*p++ != '\0')
                continue;
            while (*p == A_SKIP)
                p++;
            q = cmd-1;
        }
    }
    /*
     * No match found in the entire command table.
     */
    return (A_INVALID);
}

int keyboard_decode ()
{
    char cmd[MAX_CMDLEN+1], c;
    int  nch = 0;
    int  action = A_NONE;

    /*
     * Collect characters in a buffer.
     * Start with the one we have, and get more if we need them.
     */
    c = (char) _getch();
    if (c == '\0')
        c = '\340'; // 224
    else if (c == ESC)
    {
        cmd[nch++] = c;
        if (_kbhit ())
            c = (char) _getch ();
    }

    while (c >= 0)
    {
        cmd[nch++] = c;
        cmd[nch]   = '\0';
        action     = keyboard_search (cmd);

        if (action != A_PREFIX)
            break;
        if (!_kbhit ())
            break;
        c = (char) _getch ();
    }
    return action;
}

// Simulate Standard Microsoft Extensions under Unix
#ifdef HAVE_UNIX

static int infd = -1;

int _kbhit (void)
{
    if (infd >= 0)
    {   // Set no delay
        static struct timeval tv = {0, 0};
        fd_set rdfs;

        // See if key has been pressed
        FD_ZERO (&rdfs);
        FD_SET  (infd, &rdfs);
        if (select  (infd + 1, &rdfs, NULL, NULL, &tv) <= 0)
            return 0;
        if (FD_ISSET (infd, &rdfs))
            return 1;
    }
    return 0;
}

char _getch (void)
{
    char ch = -1;
    if (infd >= 0)
        read (infd, &ch, 1);
    return ch;
}

// Set keyboard to raw mode to getch will work
static termios term;
void keyboard_enable_raw ()
{
    // set to non canonical mode, echo off, ignore signals
    struct termios current;

    // Already open
    if (infd >= 0)
        return;

    // Determine if stdin/stderr has been redirected
    if (isatty (STDIN_FILENO))
        infd = STDIN_FILENO;
    else if (isatty (STDERR_FILENO))
        infd = STDERR_FILENO;
    else
    {   // Try opening a terminal directly
        infd = open("/dev/tty", O_RDONLY);
        if (infd < 0)
            return;
    }

    // save current terminal settings
    tcgetattr (infd, &current);

    // set to non canonical mode, echo off, ignore signals
    term = current;
    current.c_lflag &= ~(ECHO | ICANON | IEXTEN);
    current.c_cc[VMIN] = 1;
    current.c_cc[VTIME] = 0;
    tcsetattr (infd, TCSAFLUSH, &current);
}

void keyboard_disable_raw ()
{
    if (infd >= 0)
    {   // Restore old terminal settings
        tcsetattr (infd, TCSAFLUSH, &term);
        switch (infd)
        {
        case STDIN_FILENO:
        case STDERR_FILENO:
            break;
        default:
            close (infd);
        }
        infd = -1;
    }
}

#endif // HAVE_LINUX
