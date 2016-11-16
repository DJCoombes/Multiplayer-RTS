/**
* @Author	DJ Coombes
* @date		11th November 2016
* @brief	Client specific class, handles the multiplayer connection and packet distribution.
*/

#include "client.h"

#include <thread>

#include "logger.h"

Client::Client() {}

Client::~Client() {
	Disconnect();
}

bool Client::Connect() {
	if (m_connected) {
		LOG(ERRORR) << "Already connected to a server.";
		return false;
	}
	m_socket.bind(sf::Socket::AnyPort);
	LOG(INFO) << "Bound client to port: " << m_socket.getLocalPort();
	sf::Packet packet;
	SetPacketType(PacketType::CONNECT, packet);
	packet << m_playerName;
	if (m_socket.send(packet, m_serverIP, m_serverPort) != sf::Socket::Done) {
		LOG(ERRORR) << "Error connecting to game server.";
		m_socket.unbind();
		return false;
	}

	m_socket.setBlocking(false);
	packet.clear();
	sf::IpAddress serverIP;
	Port serverPort;
	sf::Clock timer;
	timer.restart();

	LOG(INFO) << "Trying to connect to: " << m_serverIP << ":" << m_serverPort;
	
	while (timer.getElapsedTime().asMilliseconds() < 10000) {
		sf::Socket::Status status = m_socket.receive(packet, serverIP, serverPort);
		if (status != sf::Socket::Done)
			continue;
		if (serverIP != m_serverIP)
			continue;
		PacketID id;
		if (!(packet >> id))
			break;
		if (id != PacketType::CONNECT)
			continue;
		m_packetHandler(id, packet, this);
		m_connected = true;
		m_socket.setBlocking(true);
		m_lastHeartBeat = m_serverTime;
		std::thread listenThread(&Client::Listen, this);
		listenThread.detach();
		return true;
	}
	LOG(INFO) << "Connecting to the server failed. Server info: " << m_serverIP << ":" << m_serverPort;
	m_socket.unbind();
	m_socket.setBlocking(true);
	return false;
}

void Client::Disconnect() {
	if (!m_connected)
		return;
	sf::Packet packet;
	SetPacketType(PacketType::DISCONNECT, packet);
	sf::Socket::Status status = m_socket.send(packet, m_serverIP, m_serverPort);
	m_connected = false;
	m_socket.unbind();
	if (status != sf::Socket::Done)
		LOG(ERRORR) << "Failed to disconnect.";
}

void Client::Listen() {
	sf::Packet packet;
	sf::IpAddress serverIP;
	Port serverPort;
	LOG(INFO) << "Listening for incoming packets...";
	while (m_connected) {
		packet.clear();
		sf::Socket::Status status = m_socket.receive(packet, serverIP, serverPort);
		if (!m_connected)
			return;
		if (status != sf::Socket::Done) {
			if (m_connected) {
				LOG(ERRORR) << "Failed to receive a packet from " << serverIP << ":" << serverPort;
				continue;
			}
			else {
				LOG(ERRORR) << "Socket is not connected to remote server";
				break;
			}
		}
		if (serverIP != m_serverIP) {
			LOG(INFO) << "Packet received from unknown source.";
			continue;
		}
		PacketID id;
		if (!(packet >> id)) {
			LOG(INFO) << "Packet received with unknown packet ID.";
			continue;
		}
		PacketType type = (PacketType)id;
		if (type < PacketType::DISCONNECT || type > PacketType::OUTOFBOUNDS) {
			LOG(ERRORR) << "Packet received with out of bounds ID.";
			continue;
		}
		if (type == PacketType::HEARTBEAT) {
			sf::Packet packet;
			SetPacketType(PacketType::HEARTBEAT, packet);
			if (m_socket.send(packet, m_serverIP, m_serverPort) != sf::Socket::Done)
				LOG(WARNING) << "Failed to send a heartbeat to game server.";
			sf::Int32 timeStamp;
			packet >> timeStamp;
			SetTime(sf::milliseconds(timeStamp));
			m_lastHeartBeat = m_serverTime;
		}
		else if (m_packetHandler) {
			m_packetHandler(id, packet, this);
		}
	}
	LOG(INFO) << "...Listening stopper.";
}

bool Client::Send(sf::Packet& packet) {
	if (!m_connected)
		return false;
	if (m_socket.send(packet, m_serverIP, m_serverPort) != sf::Socket::Done)
		return false;
	return true;
}

sf::Time& Client::GetTime() {
	return m_serverTime;
}

sf::Time& Client::GetLastHeartBeat() {
	return m_lastHeartBeat;
}

void Client::SetTime(sf::Time& time) {
	m_serverTime = time;
}

void Client::SetServerInfo(sf::IpAddress& ip, Port& port) {
	m_serverIP = ip;
	m_serverPort = port;
}

void Client::Update(sf::Time& time) {
	if (!m_connected)
		return;
	m_serverTime += time;
	if (m_serverTime.asMilliseconds() < 0) {
		m_serverTime -= sf::milliseconds(NetworkSpecifics::HIGHESTTIMESTAMP);
		m_lastHeartBeat = m_serverTime;
		return;
	}
	if (m_serverTime.asMilliseconds() - m_lastHeartBeat.asMilliseconds() >= NetworkSpecifics::CLIENTTIMEOUT) {
		LOG(WARNING) << "Server connection timed out!";
		Disconnect();
	}
}

bool Client::IsConnected() {
	return m_connected;
}

void Client::SetPlayerName(std::string& name) {
	m_playerName = name;
}

void Client::BindPacketHandler(void(*handler)(PacketID&, sf::Packet&, Client*)) {
	m_packetHandler = std::bind(handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

void Client::UnregisterPacketHandler() {
	m_packetHandler = nullptr;
}