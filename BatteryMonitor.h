#ifndef BATTERY_MONITOR_H
#define BATTERY_MONITOR_H

#include "IBreachClassifier.h"
#include "IAlertHandler.h"
#include "ControllerAlertHandler.h"
#include <unordered_map>

class BatteryMonitor {
public:
    BatteryMonitor(IBreachClassifier& classifier, IAlertHandler& controllerHandler, IAlertHandler& emailHandler)
        : classifier(classifier), controllerHandler(controllerHandler), emailHandler(emailHandler) {}

    void checkAndAlert(double temperature, bool isController) {
        BreachType breachType = classifier.classify(temperature);
        if (isController) {
            handleControllerAlert(controllerHandler, breachType);
        } else {
            handleEmailAlert(emailHandler, breachType);
        }
    }

private:
    void handleControllerAlert(IAlertHandler& alertHandler, BreachType breachType) {
        // This check is redundant since the interface should ensure the correct method is called.
        alertHandler.sendAlert(breachType);
    }

    void handleEmailAlert(IAlertHandler& alertHandler, BreachType breachType) {
        static const std::unordered_map<BreachType, std::string> breachTypeMessages = {
            {BreachType::TOO_LOW, "Hi, the temperature is too low"},
            {BreachType::TOO_HIGH, "Hi, the temperature is too high"}
        };

        auto messageLookup = breachTypeMessages.find(breachType);
        if (messageLookup != breachTypeMessages.end() && !messageLookup->second.empty()) {
            alertHandler.sendAlert(messageLookup->second);
        }
    }

    IBreachClassifier& classifier;
    IAlertHandler& controllerHandler;
    IAlertHandler& emailHandler;
};

#endif // BATTERY_MONITOR_H
