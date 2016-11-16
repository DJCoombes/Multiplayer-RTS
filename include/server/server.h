#pragma once

/**
* @Author	DJ Coombes
* @date		13th November 2016
* @brief	Server class which handles the multiplayer connection.
*/

#include <unordered_map>
#include <functional>
#include <thread>
#include <mutex>

#include <SFML/Network.hpp>
#include <SFML/System/Time.hpp>

#include "logger.h"
#include "packetTypes.h"
#include "networkSpecifics.h"

class Server;
using PacketHandler = std::function<void(ClientID&, PacketID&, sf::Packet&, Server*)>;
using TimeoutHandler = std::function<void(ClientID&)>;

struct ClientInfo {
	/*!
	  \brief Constructor.
	  \param ip IP address of the client.
	  \param port Port the client is listening on.
	  \param heartbeat Since since the last heartbeat.
	*/
	ClientInfo(sf::IpAddress& ip, Port& port, std::string& name, sf::Time heartbeat) : m_ip(ip),
		m_port(port), m_lastHeartbeat(heartbeat), m_heartbeatWaiting(false), m_heartbeatRetry(0),
		m_ping(0), m_clientName("") {
	}

	/*!
	  \brief Overloaded assignment operator, makes a copy of the client.
	  \param client The instance of the client to copy.
	*/
	ClientInfo& operator=(const ClientInfo& client) {
		m_ip				= client.m_ip;
		m_port				= client.m_port;
		m_clientName		= client.m_clientName;
		m_ping				= client.m_ping;
		m_lastHeartbeat		= client.m_lastHeartbeat;
		m_heartbeatSent		= client.m_heartbeatSent;
		m_heartbeatWaiting	= client.m_heartbeatWaiting;
		m_heartbeatRetry	= client.m_heartbeatRetry;
	}

	sf::IpAddress	m_ip; //!< IP Address of the client.
	Port			m_port; //!< Port the client is listening on.
	std::string		m_clientName; //!< Name of the player associated with this client.
	int				m_ping; //!< The clients ping.
	sf::Time		m_lastHeartbeat; //!< The last time a heartbeat was received.
	sf::Time		m_heartbeatSent; //!< The last time a heartbeat was sent
	bool			m_heartbeatWaiting; //!< True if we're waiting for the clients heartbeat.
	short			m_heartbeatRetry;
};

using Clients = std::unordered_map<ClientID, ClientInfo>;

class Server {
public:
	/*!
	  \brief Constructor.
	*/
	Server();

	/*!
	  \brief Destructor.
	*/
	~Server();

	/*!
	  \brief Bind the function that will handle the clients timeout.
	  \param T:*handler The function that will handle the clients.
	  \param instance The instance of the object from which the timeout function will be called.
	*/
	template<typename T>
	void BindTimeoutHandler(void(T::*handler)(ClientID&), T* instance) {
		m_timeoutHandler = std::bind(handler, instance, std::placeholders::_1);
	}

	/*!
	  \brief Bind the function that will handle the clients timeout.
	  \param handler The function that will handle the clients timeout.
	*/
	void BindTimeoutHandler(void(handler)(ClientID&));

	/*!
	  \brief Send a packet to a specific client.
	  \param id ID of the client to send the packet to.
	  \param packet The packet to send.
	  \return True if sent correctly, false otherwise.
	*/
	bool Send(ClientID& id, sf::Packet& packet);

	/*!
	  \brief Send a packet to a specific client IP.
	  \param ip IP address to send the packet to.
	  \param port The port the client is listening on.
	  \param packet The packet to send.
	  \return True if sent correctly, false otherwise.
	*/
	bool Send(sf::IpAddress& ip, Port& port, sf::Packet& packet);

	/*!
	  \brief Broadcast a packet to all clients.
	  \param packet The packet to broadcast.
	*/
	void Broadcast(sf::Packet& packet);

	/*!
	  \brief Listen for incoming packets, needs to be run in a thread.
	*/
	void Listen();

	/*!
	  \brief Update the server connection.
	  \param time Time since last update.
	*/
	void Update(sf::Time deltaTime);

	/*!
	  \brief Add a client to the connections.
	  \param ip IP address of the client.
	  \param port Port the client is listening on.
	  \return The new clients unique ID.
	*/
	ClientID AddClient(sf::IpAddress& ip, Port& port, std::string& name);

	/*!
	  \brief Get the client associated with a specific IP and port.
	  \param ip The ip address to check for.
	  \param port The port to check for.
	  \return The client unique ID, null if it doesn't exist.
	*/
	ClientID GetClientID(sf::IpAddress& ip, Port& port);

	/*!
	  \brief Check if a client exists.
	  \param id The unique ID to check for.
	  \return True if the client exists, false otherwise.
	*/
	bool HasClient(ClientID& id);

	/*!
	  \brief Check if a client exists using it's details.
	  \param ip The IP address to check for.
	  \param port The port to check for.
	  \return True if the client exists, false otherwise.
	*/
	bool HasClient(sf::IpAddress& ip, Port& port);

	/*!
	  \brief Get the network details for a client.
	  \param id The unique ID of the client.
	  \return The client info associated with that client.
	*/
	ClientInfo GetClientInfo(ClientID& id);

	/*!
	  \brief Remove a client using it's unique ID.
	  \param id The clients unique ID.
	  \return True if the client was removed, false otherwise.
	*/
	bool RemoveClient(ClientID& id);

	/*!
	  \brief Remove a client using it's network details.
	  \param ip The IP address of the client to remove.
	  \param port The port of the client to remove.
	  \return True if the client was removed, false otherwise.
	*/
	bool RemoveClient(sf::IpAddress& ip, Port& port);

	/*!
	  \brief Disconnect all clients.
	*/
	void DisconnectAll();

	/*!
	  \brief Start the multiplayer connection.
	  \return True if the server started correctly, false otherwise.
	*/
	bool Start();

	/*!
	  \brief Stop the multiplayer connection.
	  \return True if the server stopped correctly, false otherwise.
	*/
	bool Stop();

	/*!
	  \brief Check if the server is running.
	  \return True if it's running, false otherwise.
	*/
	bool IsRunning();

	/*!
	  \brief Get the amount of clients connected.
	  \return Int containing the amount of clients connected.
	*/
	int AmountOfClients();

	/*!
	  \brief Gets a list of the clients names.
	  \return String containing the clients names.
	*/
	std::string ListAllConnections();

	/*!
	  \brief Get the mutex which is to be used for the listen thread.
	  \return The mutex.
	*/
	std::mutex& GetMutex();

	template<class T>
	void BindPacketHandler(void(T::*handler)(ClientID&, PacketID&, sf::Packet&, Server*), T* instance) {
		if (m_packetHandler)
			UnbindPacketHandler();
		m_packetHandler = std::bind(handler, instance, std::placeholders::_1,
			std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
	}

	void UnbindPacketHandler();
private:
	/*!
	  \brief Sets up the server connection.
	*/
	void Setup();

	sf::UdpSocket	m_incoming; //!< Socket on which incoming packets will be received.
	sf::UdpSocket	m_outgoing; //!< Socket on which outgoing packets will be sent.

	ClientID		m_lastID; //!< The ID of the last player to be connected.
	Clients			m_clients; //!< A map of clients to client IDs.

	PacketHandler	m_packetHandler; //!< The function that will handle the packets.
	TimeoutHandler	m_timeoutHandler; //!< Function will handle clients timeout connection.

	bool			m_running; //!< True if the server is running, false otherwise.
	sf::Time		m_serverTime; //!< The servers time.

	std::mutex		m_mutex; //!< Mutex used for the listen thread.

	size_t			m_dataSent; //!< Total size of all packets sent.
	size_t			m_dataReceived; //!< Total size of all packets received.
};