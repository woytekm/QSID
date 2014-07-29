// --------------------------------------------------------------------------
// Advanced Linux Sound Architecture (ALSA) specific audio driver interface.
// --------------------------------------------------------------------------
/***************************************************************************
 *  $Log: audiodrv.cpp,v $
 *  Revision 1.7  2002/03/04 19:07:48  s_a_white
 *  Fix C++ use of nothrow.
 *
 *  Revision 1.6  2002/01/10 19:04:00  s_a_white
 *  Interface changes for audio drivers.
 *
 *  Revision 1.5  2001/12/11 19:38:13  s_a_white
 *  More GCC3 Fixes.
 *
 *  Revision 1.4  2001/01/29 01:17:30  jpaana
 *  Use int_least8_t instead of ubyte_sidt which is obsolete now
 *
 *  Revision 1.3  2001/01/23 21:23:23  s_a_white
 *  Replaced SID_HAVE_EXCEPTIONS with HAVE_EXCEPTIONS in new
 *  drivers.
 *
 *  Revision 1.2  2001/01/18 18:35:41  s_a_white
 *  Support for multiple drivers added.  C standard update applied (There
 *  should be no spaces before #)
 *
 *  Revision 1.1  2001/01/08 16:41:43  s_a_white
 *  App and Library Seperation
 *
 ***************************************************************************/

#include "audiodrv.h"
#ifdef   HAVE_ALSA

#include <stdio.h>
#ifdef HAVE_EXCEPTIONS
#   include <new>
#endif

Audio_ALSA::Audio_ALSA()
{
    // Reset everything.
    outOfOrder();
}

Audio_ALSA::~Audio_ALSA ()
{
    close ();
}

void Audio_ALSA::outOfOrder ()
{
    // Reset everything.
    _errorString = "None";
    _audioHandle = NULL;
}

void *Audio_ALSA::open (AudioConfig &cfg, const char *)
{
    AudioConfig tmpCfg;
    int mask, wantedFormat, format;
    int rtn;
    int card = -1, dev = 0;
   
    if (_audioHandle != NULL)
    {
        _errorString = "ERROR: Device already in use";
        return NULL;
    }

    if ((rtn = snd_pcm_open_preferred (&_audioHandle, &card, &dev, SND_PCM_OPEN_PLAYBACK)))
    {
        _errorString = "ERROR: Could not open audio device.";
        goto open_error;
    }
    
    // Transfer input parameters to this object.
    // May later be replaced with driver defaults.
    tmpCfg = cfg;

    snd_pcm_channel_params_t pp;
    snd_pcm_channel_setup_t setup;
 
    snd_pcm_channel_info_t pi;
   
    memset (&pi, 0, sizeof (pi));
    pi.channel = SND_PCM_CHANNEL_PLAYBACK;
    if ((rtn = snd_pcm_plugin_info (_audioHandle, &pi)))
    {
        _errorString = "ALSA: snd_pcm_plugin_info failed.";
        goto open_error;
    }
			
    memset(&pp, 0, sizeof (pp));
	
    pp.mode = SND_PCM_MODE_BLOCK;
    pp.channel = SND_PCM_CHANNEL_PLAYBACK;
    pp.start_mode = SND_PCM_START_FULL;
    pp.stop_mode = SND_PCM_STOP_STOP;
				     
    pp.buf.block.frag_size = pi.max_fragment_size;

    pp.buf.block.frags_max = 1;
    pp.buf.block.frags_min = 1;
   
    pp.format.interleave = 1;
    pp.format.rate = tmpCfg.frequency;
    pp.format.voices = tmpCfg.channels;
   
    // Set sample precision and type of encoding.
    if ( tmpCfg.precision == 8 )
    {
        tmpCfg.encoding = AUDIO_UNSIGNED_PCM;
        pp.format.format = SND_PCM_SFMT_U8;
    }
    if ( tmpCfg.precision == 16 )
    {
        tmpCfg.encoding = AUDIO_SIGNED_PCM;
        pp.format.format = SND_PCM_SFMT_S16_LE;
    }

    if ((rtn = snd_pcm_plugin_params (_audioHandle, &pp)) < 0)
    {
        _errorString = "ALSA: snd_pcm_plugin_params failed.";
        goto open_error;
    }
   
    if ((rtn = snd_pcm_plugin_prepare (_audioHandle, SND_PCM_CHANNEL_PLAYBACK)) < 0)
    {
        _errorString = "ALSA: snd_pcm_plugin_prepare failed.";
        goto open_error;
    }
   
    memset (&setup, 0, sizeof (setup));
    setup.channel = SND_PCM_CHANNEL_PLAYBACK;
    if ((rtn = snd_pcm_plugin_setup (_audioHandle, &setup)) < 0)
    {
        _errorString = "ALSA: snd_pcm_plugin_setup failed.";
        goto open_error;
    }

    tmpCfg.bufSize = setup.buf.block.frag_size;
#ifdef HAVE_EXCEPTIONS
    _sampleBuffer = new(std::nothrow) int_least8_t[tmpCfg.bufSize];
#else
    _sampleBuffer = new int_least8_t[tmpCfg.bufSize];
#endif

    if (!_sampleBuffer)
    {
        _errorString = "AUDIO: Unable to allocate memory for sample buffers.";
        goto open_error;
    }

    // Setup internal Config
    _settings = tmpCfg;
    // Update the users settings
    getConfig (cfg);
    return _sampleBuffer;

open_error:
    if (_audioHandle != NULL)
    {
        close ();
    }

    perror ("ALSA");
return NULL;
}

// Close an opened audio device, free any allocated buffers and
// reset any variables that reflect the current state.
void Audio_ALSA::close ()
{
    if (_audioHandle != NULL )
    {
        snd_pcm_close(_audioHandle);
        delete [] _sampleBuffer;
        outOfOrder ();
    }
}

void *Audio_ALSA::reset ()
{
    return (void *) _sampleBuffer;   
}

void *Audio_ALSA::write ()
{
    if (_audioHandle == NULL)
    {
        _errorString = "ERROR: Device not open.";
        return NULL;
    }

    snd_pcm_plugin_write (_audioHandle, _sampleBuffer, _settings.bufSize);
    return (void *) _sampleBuffer;
}

#endif // HAVE_ALSA
