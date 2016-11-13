/**
* @Author	DJ Coombes
* @date		11th November 2016
* @brief	Packet specific details and functions.
*/

#include "packetTypes.h"

void SetPacketType(const PacketType& type, sf::Packet& packet) {
	packet << type;
}