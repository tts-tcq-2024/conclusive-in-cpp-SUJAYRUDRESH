#ifndef BATTERY_MONITOR_TEST_CPP
#define BATTERY_MONITOR_TEST_CPP

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
            return BreachType::NORMAL; // Add return for NORMAL case
        }
    }
};

class MockAlertHandler : public IAlertHandler {
public:
    void sendAlert(BreachType breachType) override {
        breachTypeMessage = "feed : " + std::to_string(static_cast<int>(breachType));
    }

    void sendAlert(const std::string& message) override {
        breachTypeMessage = message;
    }

    const std::string& getBreachTypeMessage() const {
        return breachTypeMessage;
    }

private:
    std::string breachTypeMessage;
};

TEST(BatteryMonitorTest, SendsCorrectMessageToController) {
    MockAlertHandler mockHandler;
    TemperatureClassifier classifier;
    BatteryMonitor monitor(classifier, mockHandler, mockHandler);

    monitor.checkAndAlert(50, true);

    ASSERT_EQ(mockHandler.getBreachTypeMessage(), "feed : 1");  // Expecting TOO_HIGH
}

TEST(BatteryMonitorTest, SendsCorrectMessageToEmail) {
    MockOutput mockOutput;
    TemperatureClassifier classifier;
    EmailAlertHandler emailAlertHandler;
    BatteryMonitor monitor(classifier, mockOutput, emailAlertHandler);

    monitor.checkAndAlert(-5, false);  // Low temperature should trigger TOO_LOW alert

    const auto& messages = mockOutput.getMessages();
    ASSERT_EQ(messages.size(), 1);
    ASSERT_EQ(messages[0], "Hi, the temperature is too low");
}

// Main function for Google Test
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#endif // BATTERY_MONITOR_TEST_CPP
