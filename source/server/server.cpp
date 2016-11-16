/**
* @Author	DJ Coombes
* @date		13th November 2016
* @brief	Server class which handles the multiplayer connection.
*/

#include "server.h"

Server::Server() : m_running(false), m_dataSent(0),
	m_dataReceived(0) {

}

Server::~Server() {
	Stop();
}

void Server::BindTimeoutHandler(void(handler)(ClientID&)) {
	m_timeoutHandler = std::bind(handler, std::placeholders::_1);
}

bool Server::Send(ClientID& id, sf::Packet& packet) {
	try	{
		std::lock_guard<std::mutex> lock(m_mutex);
	}
	catch (const std::exception& e) {
		LOG(DEBUG) << e.what();
	}
	auto i = m_clients.find(id);
	if (i == m_clients.end())
		return false;
	if (m_outgoing.send(packet, i->second.m_ip, i->second.m_port) != sf::Socket::Done) {
		LOG(ERRORR) << "Error sending a packet";
		return false;
	}
	m_dataSent += packet.getDataSize();
	return true;
}

bool Server::Send(sf::IpAddress& ip, Port& port, sf::Packet& packet) {
	if (m_outgoing.send(packet, ip, port) != sf::Socket::Done)
		return false;
	m_dataSent += packet.getDataSize();
	return true;
}

void Server::Broadcast(sf::Packet& packet) {
	try {
		std::lock_guard<std::mutex> lock(m_mutex);
	}
	catch (const std::exception& e) {
		LOG(DEBUG) << e.what();
	}
	for (auto& i : m_clients) {
		if (m_outgoing.send(packet, i.second.m_ip, i.second.m_port) != sf::Socket::Done) {
			LOG(ERRORR) << "Error sending a packet to client: " << i.first;
			continue;
		}
		m_dataSent += packet.getDataSize();
	}
}

void Server::Listen() {
	sf::IpAddress ip;
	Port port;
	sf::Packet packet;
	ClientID client = GetClientID(ip, port);
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
		if (id == PacketType::CONNECT) {
			std::string playerName;
			packet >> playerName;
			ClientID clientID = AddClient(ip, port);
			if (clientID == -1) {
				LOG(ERRORR) << "Connected player tried to connect again.";
				break;
			}
			sf::Packet serverPacket;
			SetPacketType(PacketType::CONNECT, serverPacket);
			if (!Send(clientID, serverPacket)) {
				LOG(ERRORR) << "Cannot respond to a clients connect packet.";
				break;
			}
			LOG(INFO) << playerName << " connected.";
		}
		else if (id == PacketType::HEARTBEAT) {
			bool clientFound = false;
			try {
				std::lock_guard<std::mutex> lock(m_mutex);
			}
			catch (const std::exception& e) {
				LOG(DEBUG) << e.what();
			}
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
		}
		else if (m_packetHandler) {
			m_packetHandler(client, id, packet, this);
		}
	}
	LOG(INFO) << "Server stopped listening for incoming packets.";
}

void Server::Update(sf::Time deltaTime) {
	try {
		std::lock_guard<std::mutex> lock(m_mutex);
	}
	catch (const std::exception& e) {
		LOG(DEBUG) << e.what();
	}
	m_serverTime += deltaTime;
	if (m_serverTime.asMilliseconds() < 0) {
		m_serverTime -= sf::milliseconds((sf::Int32)NetworkSpecifics::HIGHESTTIMESTAMP);
		for (auto& client : m_clients) {
			client.second.m_lastHeartbeat = sf::milliseconds(std::abs(client.second.m_lastHeartbeat.asMilliseconds() - (sf::Int32)NetworkSpecifics::HIGHESTTIMESTAMP));
		}
	}
	for (auto i = m_clients.begin(); i != m_clients.end();) {
		sf::Int32 elapsed = m_serverTime.asMilliseconds() - i->second.m_lastHeartbeat.asMilliseconds();
		if (elapsed >= 1000) {
			if (elapsed >= (sf::Int32)NetworkSpecifics::CLIENTTIMEOUT || i->second.m_heartbeatRetry > 5) {
				if (m_timeoutHandler)
					m_timeoutHandler((ClientID)i->first);
				LOG(INFO) << "Client " << i->first << " has timed out.";
				i = m_clients.erase(i);
				continue;
			}
			if (!i->second.m_heartbeatWaiting || (elapsed >= 1000 * (i->second.m_heartbeatRetry + 1))) {
				if (i->second.m_heartbeatRetry >= 4) {
					LOG(INFO) << "Heartbeat re-try #" << i->second.m_heartbeatRetry << " for client " << i->first;
				}
				sf::Packet heartbeat;
				SetPacketType(PacketType::HEARTBEAT, heartbeat);
				heartbeat << m_serverTime.asMilliseconds();
				Send((ClientID)i->first, heartbeat);
				if (i->second.m_heartbeatRetry == 0)
					i->second.m_heartbeatSent = m_serverTime;
				i->second.m_heartbeatWaiting = true;
				++i->second.m_heartbeatRetry;

				m_dataSent += heartbeat.getDataSize();
			}
		}
		++i;
	}
}

ClientID Server::AddClient(sf::IpAddress& ip, Port& port) {
	try {
		std::lock_guard<std::mutex> lock(m_mutex);
	}
	catch (const std::exception& e) {
		LOG(DEBUG) << e.what();
	}
	for (auto& i : m_clients) {
		if (i.second.m_ip == ip && i.second.m_port == port)
			return NetworkSpecifics::NULLID;
	}
	ClientID id = m_lastID;
	ClientInfo info(ip, port, m_serverTime);
	m_clients.emplace(id, info);
	++m_lastID;
	return id;
}

ClientID Server::GetClientID(sf::IpAddress& ip, Port& port) {
	try {
		std::lock_guard<std::mutex> lock(m_mutex);
	}
	catch (const std::exception& e) {
		LOG(DEBUG) << e.what();
	}
	for (auto& i : m_clients) {
		if (i.second.m_ip == ip && i.second.m_port) {
			return i.first;
		}
	}
	return NetworkSpecifics::NULLID;
}

bool Server::HasClient(ClientID& id) {
	return (m_clients.find(id) != m_clients.end());
}

bool Server::HasClient(sf::IpAddress& ip, Port& port) {
	return(GetClientID(ip, port) >= 0);
}

ClientInfo Server::GetClientInfo(ClientID& id) {
	try {
		std::lock_guard<std::mutex> lock(m_mutex);
	}
	catch (const std::exception& e) {
		LOG(DEBUG) << e.what();
	}
	auto client = m_clients.find(id);
	return client->second;
}

bool Server::RemoveClient(ClientID& id) {
	try {
		std::lock_guard<std::mutex> lock(m_mutex);
	}
	catch (const std::exception& e) {
		LOG(DEBUG) << e.what();
	}
	auto i = m_clients.find(id);
	if (i == m_clients.end())
		return false;
	sf::Packet p;
	SetPacketType(PacketType::DISCONNECT, p);
	Send(id, p);
	m_clients.erase(i);
	return true;
}

bool Server::RemoveClient(sf::IpAddress& ip, Port& port) {
	try {
		std::lock_guard<std::mutex> lock(m_mutex);
	}
	catch (const std::exception& e) {
		LOG(DEBUG) << e.what();
	}
	for (auto i = m_clients.begin(); i != m_clients.end(); ++i) {
		if (i->second.m_ip == ip && i->second.m_port == port) {
			sf::Packet p;
			SetPacketType(PacketType::DISCONNECT, p);
			Send((ClientID)i->first, p);
			m_clients.erase(i);
			return true;
		}
	}
	return false;
}

void Server::DisconnectAll() {
	sf::Packet packet;
	SetPacketType(PacketType::DISCONNECT, packet);
	Broadcast(packet);
	try {
		std::lock_guard<std::mutex> lock(m_mutex);
	}
	catch (const std::exception& e) {
		LOG(DEBUG) << e.what();
	}
	m_clients.clear();
}

bool Server::Start() {
	if (m_running)
		return false;
	if (m_incoming.bind((unsigned short)NetworkSpecifics::SERVERPORT) != sf::Socket::Done)
		return false;
	m_outgoing.bind(sf::Socket::AnyPort);
	Setup();
	LOG(INFO) << "Incoming port: " << m_incoming.getLocalPort() << ". Outgoing port: " << m_outgoing.getLocalPort();
	std::thread listenThread(&Server::Listen, this);
	listenThread.detach();
	m_running = true;
	return true;
}

bool Server::Stop() {
	DisconnectAll();
	m_running = false;
	m_incoming.unbind();
	return true;
}

bool Server::IsRunning() {
	return m_running;
}

int Server::AmountOfClients() {
	return m_clients.size();
}

std::string Server::ListAllConnections() {
	std::string list;
	std::string delimiter = "--------------------------------------";
	list = delimiter;
	list += '\n';
	list += "ID";
	list += '\t';
	list += "Client IP:PORT";
	list += '\t'; list += '\t';
	list += "Ping";
	list += '\n';
	list += delimiter;
	list += '\n';
	for (auto &client : m_clients) {
		list += std::to_string(client.first);
		list += '\t';
		list += client.second.m_ip.toString() + ":" + std::to_string(client.second.m_port);
		list += '\t'; list += '\t';
		list += std::to_string(client.second.m_ping) + "ms.";
		list += '\n';
	}
	list += delimiter;
	list += '\n';
	list += "Total data sent: " + std::to_string(m_dataSent / 1000) + "kB. Total data received: " + std::to_string(m_dataReceived / 1000) + "kB";
	return list;
}

std::mutex& Server::GetMutex() {
	return m_mutex;
}

void Server::Setup() {
	m_lastID = 0;
	m_running = false;
	m_dataSent = 0;
	m_dataReceived = 0;
}

void Server::UnbindPacketHandler() {
	m_packetHandler = nullptr;
}