#include <string>
#include "IBreachClassifier.h"

class IAlertHandler {
public:
    virtual ~IAlertHandler() = default;
    virtual void sendAlert(BreachType breachType) = 0;
    virtual void sendAlert(const std::string& message) = 0;
};
