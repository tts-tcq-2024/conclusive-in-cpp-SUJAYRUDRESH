#ifndef I_BREACH_CLASSIFIER_H
#define I_BREACH_CLASSIFIER_H

enum class BreachType {
    TOO_LOW,
    TOO_HIGH,
    NORMAL // Add NORMAL case to enum
};

class IBreachClassifier {
public:
    virtual ~IBreachClassifier() = default;
    virtual BreachType classify(double temperatureInC) = 0;
};

#endif // I_BREACH_CLASSIFIER_H
