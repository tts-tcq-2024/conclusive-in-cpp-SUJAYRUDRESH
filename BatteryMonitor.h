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
        if (auto* controllerHandler = dynamic_cast<ControllerAlertHandler*>(&alertHandler)) {
            controllerHandler->sendAlert(breachType);
        }
    }

    void handleEmailAlert(IAlertHandler& alertHandler, BreachType breachType) {
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
