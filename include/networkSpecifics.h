#pragma once

/**
* @Author	DJ Coombes
* @date		11th November 2016
* @brief	Network specific details.
*/

#include <limits.h>

using ClientID = unsigned int;
using Port = unsigned short;

/*!
  \brief Available network details.
*/
enum NetworkSpecifics {
	NULLID				= -1,
	SERVERPORT			= 9999,
	CLIENTTIMEOUT		= 10000,
	HIGHESTTIMESTAMP	= INT_MAX
};