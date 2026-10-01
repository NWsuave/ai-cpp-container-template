#include "bmi_bmr.h"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace {

bool read_measurement(const std::string &prompt, const std::string &name,
                      double minimum, double maximum, double &value) {
  for (;;) {
    std::cout << prompt;
    std::string line;
    if (!std::getline(std::cin, line)) {
      std::cout << "\nInput ended before all values were entered. No results calculated.\n";
      return false;
    }

    std::istringstream input(line);
    double parsed = 0.0;
    std::string extra;
    if (input >> parsed && !(input >> extra) && std::isfinite(parsed) &&
        parsed >= minimum && parsed <= maximum) {
      value = parsed;
      return true;
    }

    std::cout << "Enter a finite " << name << " from " << minimum << " to "
              << maximum << ".\n";
  }
}

bool read_age(int &age_years) {
  for (;;) {
    std::cout << "Age in years (19-78): ";
    std::string line;
    if (!std::getline(std::cin, line)) {
      std::cout << "\nInput ended before all values were entered. No results calculated.\n";
      return false;
    }

    std::istringstream input(line);
    int parsed = 0;
    std::string extra;
    if (input >> parsed && !(input >> extra) && parsed >= 19 && parsed <= 78) {
      age_years = parsed;
      return true;
    }
    std::cout << "Enter a whole-number age from 19 to 78.\n";
  }
}

bool read_equation_profile(bmi_bmr::EquationProfile &profile) {
  for (;;) {
    std::cout << "Equation profile (M/F; used by the formula, not gender identity): ";
    std::string line;
    if (!std::getline(std::cin, line)) {
      std::cout << "\nInput ended before all values were entered. No results calculated.\n";
      return false;
    }

    std::istringstream input(line);
    char parsed = '\0';
    std::string extra;
    if ((input >> parsed) && !(input >> extra)) {
      if (parsed == 'M' || parsed == 'm') {
        profile = bmi_bmr::EquationProfile::Male;
        return true;
      }
      if (parsed == 'F' || parsed == 'f') {
        profile = bmi_bmr::EquationProfile::Female;
        return true;
      }
    }
    std::cout << "Enter M or F for the equation profile.\n";
  }
}

} // namespace

int main() {
  std::cout << "BMI and Basal Metabolic Rate Estimator\n"
            << "Enter metric measurements.\n";

  double height_cm = 0.0;
  double weight_kg = 0.0;
  int age_years = 0;
  bmi_bmr::EquationProfile profile = bmi_bmr::EquationProfile::Male;

  if (!read_measurement("Height in centimeters (100-250): ", "height in cm", 100.0,
                        250.0, height_cm) ||
      !read_measurement("Weight in kilograms (20-500): ", "weight in kg", 20.0,
                        500.0, weight_kg) ||
      !read_age(age_years) || !read_equation_profile(profile)) {
    return 0;
  }

  const double bmi = bmi_bmr::calculate_bmi(height_cm, weight_kg);
  const double bmr = bmi_bmr::calculate_bmr(height_cm, weight_kg, age_years, profile);

  std::cout << std::fixed << std::setprecision(2)
            << "BMI: " << bmi << " kg/m^2\n"
            << std::setprecision(0) << "Estimated BMR: " << std::round(bmr)
            << " kcal/day (Mifflin-St Jeor estimate)\n"
            << "These are estimates, not a diagnosis or a measured metabolic rate.\n";
  return 0;
}
