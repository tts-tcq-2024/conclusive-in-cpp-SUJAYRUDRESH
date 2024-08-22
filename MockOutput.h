#include "IAlertHandler.h"
#include <vector>
#include <string>

class MockOutput : public IAlertHandler {
public:
    void sendAlert(BreachType breachType) override {
        // Optionally handle BreachType if needed or leave unimplemented
    }

    void sendAlert(const std::string& message) override {
        messages.push_back(message);
    }

    const std::vector<std::string>& getMessages() const {
        return messages;
    }

private:
    std::vector<std::string> messages;
};
