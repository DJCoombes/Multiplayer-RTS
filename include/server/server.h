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

struct ClientInfo {
	/*!
	  \brief Constructor.
	  \param ip IP address of the client.
	  \param port Port the client is listening on.
	  \param connection Time since since the last connection.
	*/
	ClientInfo(sf::IpAddress& ip, Port& port, std::string& name, sf::Time connection) : m_ip(ip),
		m_port(port), m_ready(true), m_loadingComplete(false), m_lastConnection(connection), m_connectionWaiting(false), m_connectionRetry(0),
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
		m_ready				= client.m_ready;
		m_loadingComplete	= client.m_loadingComplete;
		m_lastConnection	= client.m_lastConnection;
		m_connection		= client.m_connection;
		m_connectionWaiting	= client.m_connectionWaiting;
		m_connectionRetry	= client.m_connectionRetry;
	}

	sf::IpAddress	m_ip; //!< IP Address of the client.
	Port			m_port; //!< Port the client is listening on.
	std::string		m_clientName; //!< Name of the player associated with this client.
	int				m_ping; //!< The clients ping.
	bool			m_ready; //!< True if the player's ready, false otherwise.
	bool			m_loadingComplete; //!< True if the player's loaded all resources.
	sf::Time		m_lastConnection; //!< The last time a connection was received.
	sf::Time		m_connection; //!< The last time a connection was sent
	bool			m_connectionWaiting; //!< True if we're waiting for the clients connection.
	short			m_connectionRetry;
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
	ClientInfo& GetClientInfo(ClientID& id);

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
	  \brief Get the mutex which is to be used for the listen thread.
	  \return The mutex.
	*/
	std::mutex& GetMutex();

	/*!
	  \brief Bind the function that will handle incoming packets.
	  \param handler Reference to the method for handling the packets.
	  \param instance The instance of the object from which the function will be called.
	*/
	template<class T>
	void BindPacketHandler(void(T::*handler)(ClientID&, PacketID&, sf::Packet&, Server*), T* instance) {
		if (m_packetHandler)
			UnbindPacketHandler();
		m_packetHandler = std::bind(handler, instance, std::placeholders::_1,
			std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
	}
	
	/*!
	  \brief Unbind the packet handler function.
	*/
	void UnbindPacketHandler();

	/*!
	  \brief Checks if all clients are ready.
	  \return True if all clients are ready, false otherwise.
	*/
	bool ClientsReady();

	/*!
	  \brief Checks if all clients have loaded resources.
	  \return True if all clients have loaded resources, false otherwise.
	*/
	bool ClientsLoaded();

	/*!
	  \brief Get the amount of data the server has sent.
	  \return size_t containing the amount of data that has been sent.
	*/
	size_t GetDataSent();

	/*!
	  \brief Get the amount of data the server has received.
	  \return size_t containing the amount of data that has been received.
	*/
	size_t GetDataReceieved();
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

	bool			m_running; //!< True if the server is running, false otherwise.
	sf::Time		m_serverTime; //!< The servers time.

	std::mutex		m_mutex; //!< Mutex used for the listen thread.

	size_t			m_dataSent; //!< Total size of all packets sent.
	size_t			m_dataReceived; //!< Total size of all packets received.
};