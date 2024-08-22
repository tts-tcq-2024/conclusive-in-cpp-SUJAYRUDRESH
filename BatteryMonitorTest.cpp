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
        }
    }
};

class MockAlertHandler : public IAlertHandler {
public:
    void sendAlert(BreachType breachType) override {
        breachTypeMessage = "feed : " + std::to_string(static_cast<int>(breachType));
    }
    void sendAlert(const std::string& message) override {
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
    ControllerAlertHandler controllerAlertHandler
    BatteryMonitor monitor(classifier,controllerAlertHandler ,mockHandler);

    monitor.checkAndAlert(50, true);

    ASSERT_EQ(mockHandler.getBreachTypeMessage(), "feed : 1");  // Adjust expected value
}

TEST(BatteryMonitorTest, SendsCorrectMessageToEmail) {
    MockOutput mockOutput;
    EmailAlertHandler emailAlertHandler;

    TemperatureClassifier classifier;
    BatteryMonitor monitor(classifier, emailAlertHandler, mockOutput);

    monitor.checkAndAlert(-5, false);  // Low temperature should trigger TOO_LOW alert

    const auto& messages = mockOutput.getMessages();
    ASSERT_EQ(messages.size(), 1);
    //ASSERT_EQ(messages[0], "To: a.b@c.com\nHi, the temperature is too low\n");
    ASSERT_EQ(messages[0], "Hi, the temperature is too low\n");
}

// Main function for Google Test
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#endif // BATTERY_MONITOR_TEST_CPP
