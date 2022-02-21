#include "packet.h"
#include "packet_codes.h"
#include <string>
#include <iostream>
#include <openssl/md5.h>

Packet::Packet(const std::array<uint8_t, 4096>& m_recvBuffer)
{
    m_type = m_recvBuffer[0];

    m_id = m_recvBuffer[1];

    for (std::size_t i = 0; i < 16; ++i)
        m_auth[i] = m_recvBuffer[i + 4];
}

uint8_t Packet::type() const
{
    return m_type;
}

uint8_t Packet::id() const
{
    return m_id;
}

const std::array<uint8_t, 16>& Packet::auth() const
{
    return m_auth;
}

std::vector<uint8_t> Packet::makeSendBuffer()
{
    std::vector<uint8_t> sendBuffer;
    sendBuffer.resize(20);

    if (m_type == ACCESS_REQUEST)
    {
         sendBuffer[0] = ACCESS_ACCEPT;
         std::cout << "Packet type: ACCESS_ACCEPT\n";
    }
    else
    {
        sendBuffer[0] = ACCESS_REJECT;
        std::cout << "Packet type: ACCESS_REJECT\n";
    }

    sendBuffer[1] = m_id;
    std::cout << "Packet ID: " << static_cast<size_t>(m_id) << "\n";

    sendBuffer[2] = 0;
    sendBuffer[3] = 20;

    for (size_t i = 0; i < 16; ++i)
        sendBuffer[i + 4] = m_auth[i];

    std::string secr("secret");

    for (size_t i = 0; i < 6; ++i)
        sendBuffer[i + 20] = secr[i];

    std::array<uint8_t, 16> md;

    MD5(sendBuffer.data(), 20 + secr.length(), md.data());

    for (size_t i = 0; i < 16; ++i)
        sendBuffer[i + 4] = md[i];

    return sendBuffer;
}
