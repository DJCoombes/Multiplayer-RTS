#pragma once

/**
* @Author	DJ Coombes
* @date		11th November 2016
* @brief	Client specific class, handles the multiplayer connection and packet distribution.
*/

#include <functional>
#include <mutex>

#include <SFML/Network.hpp>

#include "networkSpecifics.h"
#include "packetTypes.h"

class Client;
using PacketHandler = std::function<void(PacketID&, sf::Packet&, Client*)>;

class Client {
public:
	/*!
	  \brief Constructor.
	*/
	Client();

	/*!
	  \brief Destructor.
	*/
	~Client();

	/*!
	  \brief Try to connect to a server.
	  \return True if connected false otherwise.
	*/
	bool Connect();

	/*!
	  \brief Disconnect from a server.
	*/
	void Disconnect();

	/*!
	\brief Send a packet to the server.
	\param packet The packet to send.
	\return True if the packet send successfully, false otherwise.
	*/
	bool Send(sf::Packet& packet);

	/*!
	  \brief Listen for incoming packets.
	*/
	void Listen();

	/*!
	  \brief Get the last time the server sent a connection.
	  \return Time since last response.
	*/
	sf::Time& GetLastConnection();

	sf::Time& GetTime();

	void SetTime(sf::Time& time);

	/*!
	  \brief Set the information relating to the server.
	  \param ip IP Address of the server.
	  \param port Port number the server is listening on.
	*/
	void SetServerInfo(sf::IpAddress& ip, Port& port);

	/*!
	  \brief Set up the client by setting the function that will handle the packets.
	  \param handler The function that will handle the packets.
	  \param instance The instance of the object holding the packet handler function.
	*/
	template<class T>
	void BindPacketHandler(void(T::*handler)(PacketID&, sf::Packet&, Client*), T* instance) {
		m_packetHandler = std::bind(handler, instance, std::placeholders::_1,
			std::placeholders::_2, std::placeholders::_3);
	}

	/*!
	  \brief Set the function that will handle incoming packets.
	  \param handler Pointer to the packet handler function.
	*/
	void BindPacketHandler(void(*handler)(PacketID&, sf::Packet&, Client*));

	/*!
	  \brief Remove the function that's currently handling packets.
	*/
	void UnregisterPacketHandler();

	/*!
	  \brief Update the client.
	  \param time Time since last update.
	*/
	void Update(sf::Time& time);

	/*!
	  \brief Check if the client is connected.
	  \return True if the client is connected, false otherwise.
	*/
	bool IsConnected();

	/*!
	  \brief Set the name of the player.
	  \param name Name of the player.
	*/
	void SetPlayerName(std::string& name);

	/*!
	\brief Get the mutex which is to be used for the listen thread.
	\return The mutex.
	*/
	std::mutex& GetMutex();

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
	std::string		m_playerName; //!< Name of the player.

	sf::UdpSocket	m_socket; //!< The UDP socket.
	sf::IpAddress	m_serverIP; //!< The IP address of the server.
	Port			m_serverPort; //!< The port that the server is listening on.
	PacketHandler	m_packetHandler; //!< The function handling the packets.

	bool			m_connected; //!< Stores true / false value depending on if connected to the server.
	sf::Time		m_serverTime;
	sf::Time		m_lastConnection; //!< Last time the server sent a response.
	std::mutex		m_mutex; //!< Mutex used for the server thread.

	size_t			m_dataRecieved; //!< Amount of data received.
	size_t			m_dataSent; //!< Amount of data sent.
};