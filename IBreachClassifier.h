enum class BreachType {
    TOO_LOW,
    TOO_HIGH
};

class IBreachClassifier {
public:
    virtual ~IBreachClassifier() = default;
    virtual BreachType classify(double temperatureInC) = 0;
};
