#pragma once

/**
* @Author	DJ Coombes
* @date		20th October 2016
* @brief	Logging class, used for logging messages, errors and warnings.
*/

#include <iostream>
#include <fstream>

/*!
  \brief Types of message.
*/
enum type {
	DEBUG,
	INFO,
	WARNING,
	ERRORR
};

/*!
  \brief Configuration details.
*/
struct Config {
	bool headers = true; //!< If true display the type of message.
	type level = INFO; //!< Level of messages to display.
	bool outputFile = true; //!< Whether or not to print to file.
	bool printToConsole = true; //!< Whether or not to print to the standard output.
};

class LOG {
public:
	/*!
	  \brief Constructor.
	*/
	LOG() {}

	/*!
	  \brief Constructor.
	  \param type Type of the message.
	*/
	LOG(type type) {
		msgLevel = type;
#ifdef _DEBUG
		cfg.level = DEBUG;
#endif
		if (cfg.outputFile) {
			logFile.open("logfile.txt", std::ios_base::app);
		}
		if (cfg.headers) {
			operator<< ("[" + getLabel(type) + "] ");
		}
	}

	/*!
	  \brief Destructor.
	*/
	~LOG() {
		if (opened) {
			if (cfg.printToConsole)
				std::cout << std::endl;
			if (cfg.outputFile) {
				logFile << "\n";
				logFile.close();
			}
		}
		opened = false;
	}

	/*!
	  \brief Clears the log file.
	*/
	static void Clear() {
		std::fstream logFile;
		logFile.open("logfile.txt", std::ios::out);
		logFile.close();
	}

	/*!
	  \brief Overloaded stream operator, allows the passing of messages easily.
	  \param msg Message to display.
	  \return Pointer to the current logger.
	*/
	template<typename T>
	LOG& operator<<(const T& msg) {
		if (msgLevel >= cfg.level) {
			if (cfg.printToConsole)
				std::cout << msg;
			opened = true;
			if (cfg.outputFile) {
				logFile << msg;
			}
		}
		return *this;
	}
private:
	/*!
	  \brief Convert the type enum into a string.
	  \param type Enum type of the message.
	  \return String of the enum type.
	*/
	std::string getLabel(type type) {
		std::string label;
		switch (type) {
		case DEBUG:		label = "DEBUG";	break;
		case INFO:		label = "INFO";		break;
		case WARNING:	label = "WARNING";	break;
		case ERRORR:	label = "ERROR";	break;
		}
		return label;
	}

	bool opened = false; //!< If the logger is open.
	type msgLevel = DEBUG; //!< Level of the message.
	Config cfg; //!< Current config file.
	std::fstream logFile; //!< File to output messages to.
};