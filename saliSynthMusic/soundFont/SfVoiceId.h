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
