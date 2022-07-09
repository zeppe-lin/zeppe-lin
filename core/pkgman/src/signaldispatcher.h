//! \file      signaldispatcher.h
//! \brief     SignalHandler and SignalDispatcher classes definition

#pragma once

#include <map>

using namespace std;

//! \class  SignalHandler
//! \brief  Signal handler for the SignalDispatcher class.
//!
//! Implement this class to receive signals.
class SignalHandler
{
public:
  //! Result of a \a handleSignal() call
  enum HandlerResult
  {
    SIGNAL_NOT_HANDLED, //!<  Not handled
    EXIT,               //!<  Signal handled, exit now
    CONTINUE            //!<  Signal handled, don't exit
  };

  virtual HandlerResult handleSignal( int signalNumber ) = 0;
};

//! \class  SignalDispatcher
//! \brief  Dispatch unix signals
//!
//! Dispatches signals.
//! Singleton, use the instance() method to access the instance
//! of this class.
//!
//! Register your SignalHandler to handle signals.
class SignalDispatcher
{
public:
  //! Create the Signal Dispatcher instance
  static SignalDispatcher* instance();


  static void dispatch( int signalNumber );

  void registerHandler( SignalHandler* handler, int signalNumber );

  void unregisterHandler( int signalNumber );

protected:
  SignalDispatcher();

private:
  static SignalDispatcher* m_instance;

  map< int, SignalHandler* > m_signalHandlers;
};

// vim:sw=2:ts=2:sts=2:et:cc=79
// End of file.
