#include <gtest/gtest.h>
#include "BatteryMonitor.h"
#include "MockOutput.h"
#include "ControllerAlertHandler.h"
#include "EmailAlertHandler.h"

// Mock implementation of TemperatureClassifier for testing
class TemperatureClassifier : public IBreachClassifier {
public:
    BreachType classify(double temperatureInC) override {
        if (temperatureInC < 0) {
            return BreachType::TOO_LOW;
        } else if (temperatureInC > 45) {
            return BreachType::TOO_HIGH;
        } else {
            return BreachType::NORMAL; // Normal case added
        }
    }
};

class MockAlertHandler : public IAlertHandler {
public:
    void sendAlert(BreachType breachType) override {
        breachTypeMessage = "feed : " + std::to_string(static_cast<int>(breachType));
    }

    void sendAlert(const std::string& message) override {
        // Handle string alert case
    }

    const std::string& getBreachTypeMessage() const {
        return breachTypeMessage;
    }

private:
    std::string breachTypeMessage;
};

TEST(BatteryMonitorTest, SendsCorrectMessageToController) {
    // Create instances of the mocks
    MockAlertHandler mockControllerHandler;
    MockAlertHandler mockEmailHandler;  // Assuming emailHandler is needed but not used here
    TemperatureClassifier classifier;

    // Instantiate the BatteryMonitor with the mock objects
    BatteryMonitor monitor(classifier, mockControllerHandler, mockEmailHandler);

    // Simulate an alert for a high temperature
    monitor.checkAndAlert(50, true);  // 'true' for controller

    // Verify that the correct message was sent
    ASSERT_EQ(mockControllerHandler.getBreachTypeMessage(), "feed : 1");  // Expecting TOO_HIGH (1)
}

TEST(BatteryMonitorTest, SendsCorrectMessageToEmail) {
    // Create instances of the mocks
    MockOutput mockOutput;
    EmailAlertHandler emailAlertHandler;
    TemperatureClassifier classifier;

    // Instantiate the BatteryMonitor with the mock objects
    BatteryMonitor monitor(classifier, emailAlertHandler, mockOutput);

    // Simulate an alert for a low temperature
    monitor.checkAndAlert(-5, false);  // 'false' for email

    // Verify that the correct message was captured
    const auto& messages = mockOutput.getMessages();
    ASSERT_EQ(messages.size(), 1);  // Check that one message was sent
    ASSERT_EQ(messages[0], "Hi, the temperature is too low");  // Check the message content
}

// Main function for Google Test
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
