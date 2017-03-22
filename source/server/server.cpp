/**
* @Author	DJ Coombes
* @date		13th November 2016
* @brief	Server class which handles the multiplayer connection.
*/

#include "server.h"
// Maximum amount of players allowed, need to get rid of this.
#define MAX_PLAYERS 2

Server::Server() : m_running(false), m_dataSent(0),
	m_dataReceived(0) {

}

Server::~Server() {
	Stop();
}

bool Server::Send(ClientID& id, sf::Packet& packet) {
	// Lock the clients data.
	try	{
		std::lock_guard<std::mutex> lock(m_mutex);
	}
	catch (const std::exception& e) {
		LOG(DEBUG) << e.what();
	}
	// Find the client.
	auto i = m_clients.find(id);
	if (i == m_clients.end())
		return false;
	// Send a packet to the client.
	if (m_outgoing.send(packet, i->second.m_ip, i->second.m_port) != sf::Socket::Done) {
		LOG(ERRORR) << "Error sending a packet";
		return false;
	}
	// Increase the total amount of data sent.
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
	// Send the packet to all connected clients.
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
	LOG(INFO) << "Server listening for incoming packets...";
	while (m_running) {
		packet.clear();
		// Wait and listen for an incoming packet.
		sf::Socket::Status status = m_incoming.receive(packet, ip, port);
		if (status != sf::Socket::Done) {
			LOG(ERRORR) << "Error receiving a packet from client: " << ip << ":" << port << ". Status code: " << status;
			continue;
		}
		// Increase the total amount of data received.
		m_dataReceived += packet.getDataSize();

		PacketID id;
		if (!(packet >> id) || id < PacketType::DISCONNECT || id >= PacketType::OUTOFBOUNDS) {
			LOG(ERRORR) << "Invalid packet ID received.";
			continue;			
		}
		if (id == PacketType::CONNECT) {
			// If a player tries to connect and we're at max capacity then send them a disconnect packet rather than them timing out in 10 seconds.
			if (m_clients.size() >= MAX_PLAYERS) {
				sf::Packet serverPacket;
				SetPacketType(PacketType::DISCONNECT, serverPacket);
				Send(ip, port, serverPacket);
				continue;
			}
			// Add the new client and store their info.
			std::string playerName;
			packet >> playerName;
			ClientID clientID = AddClient(ip, port, playerName);
			if (clientID == -1) {
				LOG(ERRORR) << "Connected player tried to connect again.";
				break;
			}
			// Tell the client that we've connected with them.
			sf::Packet serverPacket;
			SetPacketType(PacketType::CONNECT, serverPacket);
			if (!Send(clientID, serverPacket)) {
				LOG(ERRORR) << "Cannot respond to a clients connect packet.";
				break;
			}
			LOG(INFO) << playerName << " connected.";
		}
		else if (id == PacketType::KEEPCONNECTION) {
			bool clientFound = false;
			try {
				std::lock_guard<std::mutex> lock(m_mutex);
			}
			catch (const std::exception& e) {
				LOG(DEBUG) << e.what();
			}
			// Find the client that sent the connection response and set the new last time.
			for (auto& i : m_clients) {
				if (i.second.m_ip != ip || i.second.m_port != port)
					continue;
				clientFound = true;
				if (!i.second.m_connectionWaiting) {
					LOG(INFO) << "Invalid connection packet received.";
					break;
				}
				i.second.m_ping = m_serverTime.asMilliseconds() - i.second.m_connection.asMilliseconds();
				i.second.m_lastConnection = m_serverTime;
				i.second.m_connectionWaiting = false;
				i.second.m_connectionRetry = 0;
				break;
			}
			if (!clientFound)
				LOG(INFO) << "Connection received from unknown client.";
		}
		else if (id == PacketType::LOADINGCOMPLETE) {
			for (auto& i : m_clients) {
				if (i.second.m_ip != ip || i.second.m_port != port)
					continue;
				i.second.m_loadingComplete = true;
				break;
			}
		}
		// If it's not a standard packet then dispatch it to the packet handler.
		else if (m_packetHandler) {
			ClientID client = GetClientID(ip, port);
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
	// Update each of the clients info with the last time they sent a response.
	m_serverTime += deltaTime;
	if (m_serverTime.asMilliseconds() < 0) {
		m_serverTime -= sf::milliseconds((sf::Int32)NetworkSpecifics::HIGHESTTIMESTAMP);
		for (auto& client : m_clients) {
			client.second.m_lastConnection = sf::milliseconds(std::abs(client.second.m_lastConnection.asMilliseconds() - (sf::Int32)NetworkSpecifics::HIGHESTTIMESTAMP));
		}
	}
	for (auto i = m_clients.begin(); i != m_clients.end();) {
		sf::Int32 elapsed = m_serverTime.asMilliseconds() - i->second.m_lastConnection.asMilliseconds();
		if (elapsed >= 1000) {
			// If the client has reached the max timeout length or connection retries then drop them.
			if (elapsed >= (sf::Int32)NetworkSpecifics::CLIENTTIMEOUT || i->second.m_connectionRetry > 5) {
				LOG(INFO) << "Client " << i->first << " has timed out.";
				i = m_clients.erase(i);
				continue;
			}
			// If they've gone over 1 second then attempt to send a connection packet.
			if (!i->second.m_connectionWaiting || (elapsed >= 1000 * (i->second.m_connectionRetry + 1))) {
				if (i->second.m_connectionRetry >= 4) {
					LOG(INFO) << "Connection re-try #" << i->second.m_connectionRetry << " for client " << i->first;
				}
				sf::Packet connection;
				SetPacketType(PacketType::KEEPCONNECTION, connection);
				connection << m_serverTime.asMilliseconds();
				Send((ClientID)i->first, connection);
				if (i->second.m_connectionRetry == 0)
					i->second.m_connection = m_serverTime;
				i->second.m_connectionWaiting = true;
				++i->second.m_connectionRetry;

				m_dataSent += connection.getDataSize();
			}
		}
		++i;
	}
}

ClientID Server::AddClient(sf::IpAddress& ip, Port& port, std::string& name) {
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
	ClientInfo info(ip, port, name, m_serverTime);
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

ClientInfo& Server::GetClientInfo(ClientID& id) {
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
	// Pass the function to be used for listening for packets.
	std::thread listenThread(&Server::Listen, this);
	// Deatch the thread from this thread.
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

bool Server::ClientsReady() {
	try {
		std::lock_guard<std::mutex> lock(m_mutex);
	}
	catch (const std::exception& e) {
		LOG(DEBUG) << e.what();
	}

	if (m_clients.size() < MAX_PLAYERS)
		return false;
	for (auto& i : m_clients) {
		if (!i.second.m_ready)
			return false;
	}
	return true;
}

bool Server::ClientsLoaded() {
	try {
		std::lock_guard<std::mutex> lock(m_mutex);
	}
	catch (const std::exception& e) {
		LOG(DEBUG) << e.what();
	}
	if (m_clients.size() < MAX_PLAYERS)
		return false;
	for (auto& i : m_clients) {
		if (!i.second.m_loadingComplete)
		return false;
	}
	return true;
}
