#ifndef EMAIL_ALERT_HANDLER_H
#define EMAIL_ALERT_HANDLER_H

#include "IAlertHandler.h"
#include <iostream>

class EmailAlertHandler : public IAlertHandler {
public:
    void sendAlert(BreachType breachType) override {
        // Optionally handle BreachType if needed or leave unimplemented
    }

    void sendAlert(const std::string& message) override {
        const char* recipient = "a.b@c.com";
        std::cout << "To: " << recipient << std::endl;
        std::cout << message << std::endl;
    }
};

#endif // EMAIL_ALERT_HANDLER_H
