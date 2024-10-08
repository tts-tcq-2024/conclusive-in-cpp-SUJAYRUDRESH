#ifndef CONTROLLER_ALERT_HANDLER_H
#define CONTROLLER_ALERT_HANDLER_H

#include "IAlertHandler.h"
#include <iostream>

class ControllerAlertHandler : public IAlertHandler {
public:
    void sendAlert(BreachType breachType) override {
        const unsigned short header = 0xfeed;
        std::cout << std::hex << header << " : " << static_cast<int>(breachType) << std::endl;
    }

    void sendAlert(const std::string& message) override {
        // Optionally implement or leave as-is
    }
};

#endif // CONTROLLER_ALERT_HANDLER_H
