#include "bmr_factory.h"

BMRCalculator* BMRCalculatorFactory::createCalculator(const std::string& type) {
    if (type == "mifflin") return new MifflinStJeorCalculator();
    if (type == "harris")  return new HarrisBenedictCalculator();
    if (type == "katch")   return new KatchMcArdleCalculator();
    return nullptr;
}
