#include <gtest/gtest.h>
#include "BatteryMonitor.h"
#include "MockOutput.h"
#include "ControllerAlertHandler.h"
#include "EmailAlertHandler.h"
#include "TemperatureClassifier.h"

TEST(BatteryMonitorTest, SendsCorrectMessageToController) {
    MockOutput mockOutput;
    ControllerAlertHandler controllerAlertHandler;

    TemperatureClassifier classifier;
    BatteryMonitor monitor(classifier, controllerAlertHandler, mockOutput);

    monitor.checkAndAlert(50, true);  // High temperature should trigger TOO_HIGH alert

    const auto& messages = mockOutput.getMessages();
    ASSERT_EQ(messages.size(), 1);
    ASSERT_EQ(messages[0], "feed : 1\n");  // Ensure this output format matches your implementation
}

TEST(BatteryMonitorTest, SendsCorrectMessageToEmail) {
    MockOutput mockOutput;
    EmailAlertHandler emailAlertHandler(mockOutput);

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
