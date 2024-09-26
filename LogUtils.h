#ifndef LOGUTILS_H
#define LOGUTILS_H

#include <string>
#include <vector>

// Declare a vector to hold debug messages
extern std::vector<std::string> debugMessages;

// Function to log messages to the UI
void LogToUI(const std::string& message);

#endif // LOGUTILS_H