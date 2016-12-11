/**
* @Author	DJ Coombes
* @date		21st October 2016
* @brief	Communicator used for managing system messages.
*/

#include "communicator.h"

void Communicator::Add(Observer* observer) {
	if (!Has(observer))
		m_observers.emplace_back(observer);
}

void Communicator::Remove(Observer* observer) {
	auto temp = std::find(m_observers.begin(), m_observers.end(), observer);
	if (temp == m_observers.end())
		return;
	m_observers.erase(temp);
}

bool Communicator::Has(Observer* observer) {
	auto temp = std::find(m_observers.begin(), m_observers.end(), observer);
	if (temp == m_observers.end())
		return false;
	return true;
}

void Communicator::Broadcast(Message& message) {
	for (auto& i : m_observers) {
		i->Notify(message);
	}
}