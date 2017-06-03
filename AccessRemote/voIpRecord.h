#pragma once

/*
	MERCI SFML POUR LE COPIAGE!
*/

#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <iostream>


const sf::Uint8 audioData = 1;
const sf::Uint8 endOfStream = 2;

class voIpRecord : public sf::SoundRecorder
{
public:

	voIpRecord(const sf::IpAddress& host, const unsigned short port) : m_host(host), m_port(port)
	{
	}

	sf::IpAddress  m_host;   ///< Address of the remote host
	unsigned short m_port;   ///< Remote port
	sf::TcpSocket m_socket; ///< Socket used to communicate with the server

	~voIpRecord();

private:
	virtual bool onStart()
	{
		if (m_socket.connect(m_host, m_port) == sf::Socket::Done)
		{
			std::cout << "Connected to server " << m_host << std::endl;
			return true;
		}
		else
		{
			std::cout << "ERROR: m_host: " << m_host << "   m_port: " << m_port << std::endl;
			return false;
		}
	}

	virtual bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount)
	{
		// Pack the audio samples into a network packet
		sf::Packet packet;
		packet << audioData;
		packet.append(samples, sampleCount * sizeof(sf::Int16));

		// Send the audio packet to the server
		return m_socket.send(packet) == sf::Socket::Done;
	}

	virtual void onStop()
	{
		// Send a "end-of-stream" packet
		sf::Packet packet;
		packet << endOfStream;
		m_socket.send(packet);

		// Close the socket
		m_socket.disconnect();
	}
};

