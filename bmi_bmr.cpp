#include "bmi_bmr.h"

#include <cmath>
#include <stdexcept>

namespace bmi_bmr {
namespace {

void validate_measurements(double height_cm, double weight_kg) {
  if (!std::isfinite(height_cm) || height_cm <= 0.0) {
    throw std::invalid_argument("height must be a finite positive number");
  }
  if (!std::isfinite(weight_kg) || weight_kg <= 0.0) {
    throw std::invalid_argument("weight must be a finite positive number");
  }
}

} // namespace

double calculate_bmi(double height_cm, double weight_kg) {
  validate_measurements(height_cm, weight_kg);

  const double height_m = height_cm / 100.0;
  const double bmi = weight_kg / (height_m * height_m);
  if (!std::isfinite(bmi) || bmi <= 0.0) {
    throw std::invalid_argument("measurements are outside the calculable range");
  }
  return bmi;
}

double calculate_bmr(double height_cm, double weight_kg, int age_years,
                     EquationProfile profile) {
  validate_measurements(height_cm, weight_kg);
  if (age_years <= 0) {
    throw std::invalid_argument("age must be a positive number of years");
  }

  double profile_constant = 0.0;
  switch (profile) {
  case EquationProfile::Male:
    profile_constant = 5.0;
    break;
  case EquationProfile::Female:
    profile_constant = -161.0;
    break;
  default:
    throw std::invalid_argument("unsupported equation profile");
  }

  const double bmr = 10.0 * weight_kg + 6.25 * height_cm - 5.0 * age_years +
                     profile_constant;
  if (!std::isfinite(bmr) || bmr <= 0.0) {
    throw std::invalid_argument("measurements are outside the calculable range");
  }
  return bmr;
}

} // namespace bmi_bmr
