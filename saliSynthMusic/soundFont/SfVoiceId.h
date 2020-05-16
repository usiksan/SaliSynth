/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   SfVoiceId structure which unites bank and programm numbers to the whole ident
*/
#ifndef SFVOICEID_H
#define SFVOICEID_H

//!
//! \brief The SfVoiceId union Full voice id as single int value.
//! Voice id parts are: programm + bank lsb + bank msb
//!
union SfVoiceId {
    int mVoiceId;
    struct {
        unsigned mProgramm : 7;
        unsigned mBankLsb : 7;
        unsigned mBankMsb : 7;
      };
  };


#endif // SFVOICEID_H
