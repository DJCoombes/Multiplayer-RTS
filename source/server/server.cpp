#include "server.h"

Server::Server(void(*handler)(sf::IpAddress&, Port&, PacketID&, sf::Packet&, Server*)) {
	m_packetHandler = std::bind(handler, std::placeholders::_1, std::placeholders::_2,
		std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
}

Server::~Server() {
	Stop();
}

void Server::BindTimeoutHandler(void(handler)(ClientID&)) {
	m_timeoutHandler = std::bind(handler, std::placeholders::_1);
}

bool Server::Send(ClientID& id, sf::Packet& packet) {
	m_mutex.lock();
	auto i = m_clients.find(id);
	if (i == m_clients.end())
		return false;
	if (m_outgoing.send(packet, i->second.m_ip, i->second.m_port) != sf::Socket::Done) {
		LOG(ERRORR) << "Error sending a packet";
		return false;
	}
	m_dataSent += packet.getDataSize();
	m_mutex.unlock();
	return true;
}

bool Server::Send(sf::IpAddress& ip, Port& port, sf::Packet& packet) {
	if (m_outgoing.send(packet, ip, port) != sf::Socket::Done)
		return false;
	m_dataSent += packet.getDataSize();
	return true;
}

void Server::Broadcast(sf::Packet& packet) {
	m_mutex.lock();
	for (auto& i : m_clients) {
		if (m_outgoing.send(packet, i.second.m_ip, i.second.m_port) != sf::Socket::Done) {
			LOG(ERRORR) << "Error sending a packet to client: " << i.first;
			continue;
		}
		m_dataSent += packet.getDataSize();
	}
	m_mutex.unlock();
}

void Server::Listen() {
	sf::IpAddress ip;
	Port port;
	sf::Packet packet;
	LOG(INFO) << "Server listening for incoming packets...";
	while (m_running) {
		packet.clear();
		sf::Socket::Status status = m_incoming.receive(packet, ip, port);
		if (status != sf::Socket::Done) {
			LOG(ERRORR) << "Error receiving a packet from client: " << ip << ":" << port << ". Status code: " << status;
			continue;
		}

		m_dataReceived += packet.getDataSize();

		PacketID id;
		if (!(packet >> id) || id < PacketType::DISCONNECT || id >= PacketType::OUTOFBOUNDS) {
			LOG(ERRORR) << "Invalid packet ID received.";
			continue;			
		}
		if (id == PacketType::HEARTBEAT) {
			bool clientFound = false;
			m_mutex.lock();
			for (auto& i : m_clients) {
				if (i.second.m_ip != ip || i.second.m_port != port)
					continue;
				clientFound = true;
				if (!i.second.m_heartbeatWaiting) {
					LOG(INFO) << "Invalid heartbeat packet received.";
					break;
				}
				i.second.m_ping = m_serverTime.asMilliseconds() - i.second.m_heartbeatSent.asMilliseconds();
				i.second.m_lastHeartbeat = m_serverTime;
				i.second.m_heartbeatWaiting = false;
				i.second.m_heartbeatRetry = 0;
				break;
			}
			if (!clientFound)
				LOG(INFO) << "Heartbeat received from unknown client.";
			m_mutex.unlock();
		}
		else if (m_packetHandler) {
			m_packetHandler(ip, port, id, packet, this);
		}
	}
	LOG(INFO) << "Server stopped listening for incoming packets.";
}