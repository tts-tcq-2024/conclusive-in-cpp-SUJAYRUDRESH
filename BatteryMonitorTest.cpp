#ifndef BATTERY_MONITOR_TEST_CPP
#define BATTERY_MONITOR_TEST_CPP
/*
#include <gtest/gtest.h>
#include "BatteryMonitor.h"
#include "MockOutput.h"
#include "ControllerAlertHandler.h"
#include "EmailAlertHandler.h"
*/
#include "BatteryMonitor.h"          // For BatteryMonitor class
#include "ControllerAlertHandler.h"  // For ControllerAlertHandler class
#include "IAlertHandler.h"           // For IAlertHandler interface
#include "MockOutput.h"        // For MockAlertHandler class (a mock class to capture alerts)
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
/*
TEST(BatteryMonitorTest, SendsCorrectMessageToController) {
    MockOutput mockOutput;
    ControllerAlertHandler controllerAlertHandler;

    TemperatureClassifier classifier;
    BatteryMonitor monitor(classifier, controllerAlertHandler, mockOutput);

    monitor.checkAndAlert(50, true);  // High temperature should trigger TOO_HIGH alert

    const auto& messages = mockOutput.getMessages();
    //ASSERT_EQ(messages.size(), 1);
    ASSERT_EQ(messages[0], "feed : 1\n");  // Ensure this output format matches your implementation
}
*/
TEST(BatteryMonitorTest, SendsCorrectMessageToController) {
    MockAlertHandler mockAlertHandler;
    ControllerAlertHandler controllerAlertHandler;

    TemperatureClassifier classifier;
    BatteryMonitor monitor(classifier, controllerAlertHandler, mockAlertHandler);

    monitor.checkAndAlert(50, true);  // High temperature should trigger TOO_HIGH alert

    const auto& messages = mockAlertHandler.getMessages();

    // Debugging output to check if sendAlert was called
    std::cout << "Messages received: " << messages.size() << std::endl;
    for (const auto& message : messages) {
        std::cout << "Message: " << message << std::endl;
    }

    ASSERT_EQ(messages.size(), 1);  // Ensure exactly one message was sent
    ASSERT_EQ(messages[0], "feed : 1\n");  // Ensure this output format matches your implementation
}

TEST(BatteryMonitorTest, SendsCorrectMessageToEmail) {
    MockOutput mockOutput;
    EmailAlertHandler emailAlertHandler;

    TemperatureClassifier classifier;
    BatteryMonitor monitor(classifier, emailAlertHandler, mockOutput);

    monitor.checkAndAlert(-5, false);  // Low temperature should trigger TOO_LOW alert

    const auto& messages = mockOutput.getMessages();
    ASSERT_EQ(messages.size(), 1);
    ASSERT_EQ(messages[0], "To: a.b@c.com\nHi, the temperature is too low\n");
}

// Main function for Google Test
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#endif // BATTERY_MONITOR_TEST_CPP
