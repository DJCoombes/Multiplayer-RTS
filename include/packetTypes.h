#pragma once

/**
* @Author	DJ Coombes
* @date		11th November 2016
* @brief	Packet specific details and functions.
*/

#include <SFML/Network/Packet.hpp>

using PacketID = sf::Int8;

/*!
  \brief Types of packets that can be sent or received.
*/
enum PacketType {
	DISCONNECT = -1,
	CONNECT,
	CLIENTREADY,
	STARTLOADING,
	LOADINGCOMPLETE,
	START,
	HEARTBEAT,
	ENTITYCREATION,
	ENTITYUPDATE,
	UPDATE,
	OUTOFBOUNDS
};

/*!
  \brief Set the type of packet.
  \param type Type of the packet.
  \param packet The packet to set the type of.
*/
void SetPacketType(const PacketType& type, sf::Packet& packet);