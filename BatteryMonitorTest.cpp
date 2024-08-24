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


TEST(BatteryMonitorTest, SendsCorrectMessageToControllerTooLow) {
    MockBreachClassifier mockClassifier;
    mockClassifier.breachTypeToReturn = BreachType::TOO_LOW;
    MockControllerAlertHandler mockControllerHandler;
    MockEmailAlertHandler mockEmailHandler;

    BatteryMonitor monitor(mockClassifier, mockControllerHandler, mockEmailHandler);

    monitor.checkAndAlert(-5, true);

    ASSERT_EQ(mockControllerHandler.getBreachTypeMessage(), "feed : 0");  // TOO_LOW
}

TEST(BatteryMonitorTest, SendsCorrectMessageToControllerTooHigh) {
    MockBreachClassifier mockClassifier;
    mockClassifier.breachTypeToReturn = BreachType::TOO_HIGH;
    MockControllerAlertHandler mockControllerHandler;
    MockEmailAlertHandler mockEmailHandler;

    BatteryMonitor monitor(mockClassifier, mockControllerHandler, mockEmailHandler);

    monitor.checkAndAlert(50, true);

    ASSERT_EQ(mockControllerHandler.getBreachTypeMessage(), "feed : 1");  // TOO_HIGH
}

TEST(BatteryMonitorTest, SendsCorrectMessageToEmailTooLow) {
    MockBreachClassifier mockClassifier;
    mockClassifier.breachTypeToReturn = BreachType::TOO_LOW;
    MockControllerAlertHandler mockControllerHandler;
    MockEmailAlertHandler mockEmailHandler;

    BatteryMonitor monitor(mockClassifier, mockControllerHandler, mockEmailHandler);

    monitor.checkAndAlert(-5, false);

    ASSERT_EQ(mockEmailHandler.getEmailMessage(), "Hi, the temperature is too low");
}

TEST(BatteryMonitorTest, SendsCorrectMessageToEmailTooHigh) {
    MockBreachClassifier mockClassifier;
    mockClassifier.breachTypeToReturn = BreachType::TOO_HIGH;
    MockControllerAlertHandler mockControllerHandler;
    MockEmailAlertHandler mockEmailHandler;

    BatteryMonitor monitor(mockClassifier, mockControllerHandler, mockEmailHandler);

    monitor.checkAndAlert(50, false);

    ASSERT_EQ(mockEmailHandler.getEmailMessage(), "Hi, the temperature is too high");
}

TEST(BatteryMonitorTest, NoAlertSentForNormalTemperature) {
    MockBreachClassifier mockClassifier;
    mockClassifier.breachTypeToReturn = BreachType::NORMAL;
    MockControllerAlertHandler mockControllerHandler;
    MockEmailAlertHandler mockEmailHandler;

    BatteryMonitor monitor(mockClassifier, mockControllerHandler, mockEmailHandler);

    monitor.checkAndAlert(25, true);

    ASSERT_EQ(mockControllerHandler.getBreachTypeMessage(), "");
    ASSERT_EQ(mockEmailHandler.getEmailMessage(), "");
}

// Main function for Google Test
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
